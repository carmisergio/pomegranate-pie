#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <memory>

#include "asio.hpp"
#include "tsqueue.hpp"

struct NetFrame
{
    uint32_t length;
    std::vector<uint8_t> payload;

    // Constructor
    NetFrame(std::string data)
    {
        // Copy payload
        std::vector<uint8_t> new_payload(data.begin(), data.end());
        payload = new_payload;

        // Set length
        length = data.length();
    }
    NetFrame(){};

    // Resizes payload to lenggh
    void auto_resize()
    {
        this->payload.resize(this->length);
    };

    // Get message as bytes
    std::vector<uint8_t> as_bytes()
    {
        std::vector<uint8_t> bytes(sizeof(length) + payload.size());

        // Copy length
        std::memcpy(bytes.data(), &length, sizeof(uint32_t));
        // Copy payload
        std::memcpy(bytes.data() + sizeof(uint32_t), payload.data(), payload.size());

        return bytes;
    };

    // Get payload as string
    std::string payload_string()
    {
        std::string string;
        string.resize(this->length);

        std::memcpy(string.data(), this->payload.data(), this->length);

        return string;
    }

    friend std::ostream &operator<<(std::ostream &os, const NetFrame &frame)
    {
        os << "NetFrame[length: " << frame.length << ", payload: ";

        for (int i = 0; i < frame.length; i++)
        {
            os << std::hex << std::setfill('0') << std::setw(2) << (int)frame.payload[i];

            if (i != frame.length - 1)
                std::cout << ", ";
        }

        os << "]";

        return os;
    }
};

class ClientConnection
{
public:
    // Constructor
    ClientConnection(std::string host, std::string service)
    {
        // Save configuration
        this->host = host;
        this->service = service;

        // Construct ASIO context and socket
        this->io_context = std::make_shared<asio::io_context>();
        this->socket = std::make_shared<asio::ip::tcp::socket>(
            *this->io_context);

        // Socket is not connected at the start
        this->connected = false;

        // Run main loop
        this->reader_thread = std::make_shared<std::thread>(&ClientConnection::reader, this);
        this->writer_thread = std::make_shared<std::thread>(&ClientConnection::writer, this);
        this->connector_thread = std::make_shared<std::thread>(&ClientConnection::connector, this);
    }

    // Send frame
    void send(NetFrame frame)
    {
        this->outgoing_queue.push(frame);
    };

    // Get received frames
    NetFrame get()
    {
        return this->incoming_queue.pop();
    }

    // Get if connected
    bool is_connected()
    {
        return this->connected;
    }

private:
    // Client thread
    std::shared_ptr<std::thread> writer_thread;
    std::shared_ptr<std::thread> reader_thread;
    std::shared_ptr<std::thread> connector_thread;

    // ASIO
    std::shared_ptr<asio::io_context> io_context;
    std::shared_ptr<asio::ip::tcp::socket> socket;

    // Configuration
    std::string host;
    std::string service;

    // Connection status
    bool connected;

    // Queues
    TSQueue<NetFrame> incoming_queue;
    TSQueue<NetFrame> outgoing_queue;

    // Reader loop
    void reader()
    {
        NetFrame frame;
        while (true)
        {

            // Wait for socket to be connected
            while (!this->connected)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

            try
            {
                frame = receive_frame();
            }
            catch (std::exception e)
            {
                this->connected = false;
                continue;
            }

            this->incoming_queue.push(frame);
        }
    }

    // Writer loop
    void writer()
    {
        NetFrame frame;
        while (true)
        {
            frame = this->outgoing_queue.pop();

            // Wait for socket to be connected
            while (!this->connected)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

            try
            {
                send_frame(frame);
            }
            catch (std::exception e)
            {
                // Put message back in queue
                this->outgoing_queue.push_front(frame);

                this->connected = false;
            }
        }
    }

    // Connector loop
    void connector()
    {
        while (true)
        {
            if (!this->connected)
                do_connect();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    // Functions
    void do_connect()
    {
        asio::error_code ec;

        // Construct objects used for hostname resolution
        asio::ip::tcp::resolver resolver(*this->io_context);
        asio::ip::tcp::resolver::query query(this->host, this->service);

        // Repeat until connection is succesful
        while (!this->connected)
        {
            // Resolve hostname
            asio::ip::basic_resolver_results<asio::ip::tcp> endpoints;
            endpoints = resolver.resolve(query, ec);

            if (ec)
            {
                // std::cout << ec.message() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }

            // Connect to endpoint
            asio::connect(*this->socket, endpoints.begin(), ec);
            if (ec)
            {
                // std::cout << ec.message() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }

            // std::cout << "Connected!" << std::endl;

            this->connected = true;
        }
    }

    NetFrame receive_frame()
    {
        asio::error_code ec;

        NetFrame frame;

        // Read length
        asio::read(*this->socket, asio::buffer(&frame.length, sizeof(frame.length)), ec);

        if (ec)
        {
            // std::cout << ec.message() << std::endl;
            this->connected = false;

            throw std::logic_error("Error");
        }

        frame.auto_resize();

        // Read message
        asio::read(*this->socket, asio::buffer(frame.payload, frame.length));

        std::cout << "Pusing frame: " << frame.payload_string() << std::endl;

        return frame;
    }

    void send_frame(NetFrame frame)
    {
        asio::error_code ec;
        std::vector<uint8_t> bytes = frame.as_bytes();

        // Perform write
        asio::write(*this->socket, asio::buffer(bytes, bytes.size()), ec);

        if (ec)
        {
            throw std::logic_error("Error");
        }
    };
};

// TODO figure out registering workers