#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include <list>
#include <memory>
#include <atomic>

#include "asio.hpp"
#include "tsqueue.hpp"
#include "client_frame.hpp"

class ServerConnection : public std::enable_shared_from_this<ServerConnection>
{
public:
    ServerConnection(
        std::shared_ptr<asio::ip::tcp::socket> socket,
        std::function<void(std::shared_ptr<ServerConnection>)> add_to_con_removal_list,
        std::function<void(std::string, std::shared_ptr<ServerConnection> connection)> message_received_callback)
        : socket(socket),
          add_to_con_removal_list(add_to_con_removal_list),
          message_received_callback(message_received_callback),
          reader_thr(&ServerConnection::reader, this),
          writer_thr(&ServerConnection::writer, this){};

    bool is_running()
    {
        return this->run.load();
    }

    /**
     * Send message
     *
     * @param msg messsage to be sent
     */
    void
    send_message(std::string msg)
    {
        this->outgoing.push(msg);
    }

    ~ServerConnection()
    {
        // std::cout << "ServerConnection destructor called!" << std::endl;

        this->run = false;

        if (this->reader_thr.joinable())
            this->reader_thr.join();

        this->outgoing.push("");
        if (this->writer_thr.joinable())
            this->writer_thr.join();
    }

private:
    void reader()
    {
        asio::error_code ec;

        while (this->run.load())
        {
            read_message(ec);

            if (ec)
            {
                std::cout << "Error on read: " << ec.message() << std::endl;

                this->run = false;

                add_to_con_removal_list(shared_from_this());
            }
        }
    }

    void writer()
    {
        asio::error_code ec;
        std::string msg;

        while (this->run.load())
        {
            msg = this->outgoing.pop(); // Get message to send

            asio::write(*this->socket, asio::buffer(NetFrame(msg).as_bytes()), ec);

            if (ec)
            {
                std::cout << "Error on write: " << ec.message() << std::endl;

                this->run = false;

                add_to_con_removal_list(shared_from_this());
            }
        }
    }

    // Read message from the socket
    void read_message(asio::error_code &ec)
    {
        NetFrame frame;

        // Read length of message
        asio::read(*this->socket, asio::buffer(&frame.length, sizeof(frame.length)), ec);

        if (ec)
            return;

        // Prepare buffer for message
        frame.auto_resize();

        // Read message
        asio::read(*this->socket, asio::buffer(frame.payload), ec);

        if (ec)
            return;

        // std::cout << "[" << this->socket->remote_endpoint().address() << "]: " << frame.payload << std::endl;
        this->message_received_callback(frame.payload, shared_from_this());
    }

    std::shared_ptr<asio::ip::tcp::socket> socket; // Socket to the client

    std::atomic<bool> run{true};

    // Callbacks
    std::function<void(std::shared_ptr<ServerConnection>)> add_to_con_removal_list;
    std::function<void(std::string, std::shared_ptr<ServerConnection>)> message_received_callback;

    std::thread reader_thr;
    std::thread writer_thr;

    TSQueue<std::string> outgoing;
};

class Server
{
public:
    /**
     * Constructor
     */
    Server(
        int port,
        std::function<void(std::string, std::shared_ptr<ServerConnection>)> message_received_callback)
        : acceptor(this->io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
          message_received_callback(message_received_callback),
          acceptor_thread(&Server::acceptor_function, this),
          con_remover_thread(&Server::con_remover, this){};

    /**
     * Send message to all connected clients
     */
    void send_all(std::string msg)
    {
        for (auto con : this->active_connections)
        {
            con->send_message(msg);
        }
    }

private:
    void acceptor_function()
    {
        while (true)
        {

            // Create new socket
            std::shared_ptr<asio::ip::tcp::socket> socket = std::make_shared<asio::ip::tcp::socket>(this->io_ctx);

            // Accept new connection and bind it to the previously create socket
            this->acceptor.accept(*socket);

            std::cout << "Received connection from " << socket->remote_endpoint().address() << std::endl;

            // Create connection around this socket
            this->active_connections.push_back(std::make_shared<ServerConnection>(
                socket,
                [this](std::shared_ptr<ServerConnection> to_remove)
                { this->con_removal_queue.push(to_remove); },
                this->message_received_callback));
        }
    }

    // Function responsible for deconstructing disconnected connections
    void con_remover()
    {
        while (true)
        {
            std::shared_ptr<ServerConnection> to_remove = this->con_removal_queue.pop();
            this->active_connections.remove(to_remove);
        }
    }

    asio::io_context io_ctx;
    asio::ip::tcp::acceptor acceptor;

    std::thread acceptor_thread;
    std::thread con_remover_thread;

    TSQueue<std::shared_ptr<ServerConnection>> con_removal_queue;

    std::list<std::shared_ptr<ServerConnection>> active_connections;

    // Callbacks
    std::function<void(std::string, std::shared_ptr<ServerConnection>)> message_received_callback;
};