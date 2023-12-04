
#pragma once

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

#include <iostream>

#include "asio.hpp"
#include "tsqueue.hpp"

#include "client_frame.hpp"

class ClientConnection
{
public:
    /**
     * Constructor
     *
     * @param host host to connect to
     * @param port port to which to connect
     * @param message_received_callback called when a message is received
     */
    ClientConnection(std::string host, short port,
                     std::function<void(std::string)> message_received_callback,
                     std::function<void()> connected_callback)
        : socket(io_ctx), reader_thr(&ClientConnection::reader, this),
          writer_thr(&ClientConnection::writer, this)
    {

        // Set KEEP_ALIVE sockopt
        set_keep_alive();

        // Save config
        this->host = host;
        this->port = port;

        this->message_received_callback = message_received_callback;
        this->connected_callback = connected_callback;

        this->is_connected = false;
    }

    /**
     * Destructor
     */
    ~ClientConnection()
    {
        // If open, send all messages and close
        // else just close
        if (this->is_connected)
        {
            // Wait for messages to be sent
            // TODO possible condition where the socket disconnects in this procedure
            while (!this->outgoing.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        // Close socket
        this->socket.close();

        // Join threads
        if (this->reader_thr.joinable())
            this->reader_thr.join();

        if (this->writer_thr.joinable())
            this->writer_thr.join();
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

private:
    void reader()
    {
        asio::error_code ec;

        while (true)
        {
            // If socket is not disconnected, read messages
            if (this->is_connected)
            {
                read_message(ec);

                if (ec)
                {
                    // std::cout << "Error on read: " << ec.message() << std::endl;

                    this->is_connected = false;
                }
            }
            else
            {
                // Connect
                do_connect();
            }
        }
    }

    void writer()
    {
        asio::error_code ec;
        std::string msg;

        while (this->run)
        {
            msg = this->outgoing.pop(); // Get message to send

            // Only write if socket is open
            if (this->is_connected)
            {
                asio::write(this->socket, asio::buffer(NetFrame(msg).as_bytes()), ec);

                if (ec)
                {
                    std::cout << "Error on write: " << ec.message() << std::endl;

                    this->is_connected = false;

                    // Push message that we were writing back to the queue
                    this->outgoing.push_front(msg);
                }
            }
            else
            {
                this->outgoing.push_front(msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    }

    // Connect the socket
    void do_connect()
    {
        std::cout << "[NETWORK] Disconnected! " << std::endl;

        asio::error_code ec;

        // Construct objects used for hostname resolution
        asio::ip::tcp::resolver resolver(this->io_ctx);
        asio::ip::tcp::resolver::query query(this->host, std::to_string(this->port));
        asio::ip::basic_resolver_results<asio::ip::tcp> endpoints;

        bool first_try = true;

        // Close socket
        this->socket.close(ec);

        // Repeat until connection is succesful
        do
        {
            this->is_connected = false;

            if (!first_try)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            first_try = false;

            std::cout << "[NETWORK] Trying to connect to  " << host << ":" << std::to_string(port) << std::endl;

            // Resolve hostname
            endpoints = resolver.resolve(query, ec);
            if (ec)
            {
                std::cout << "[NETWORK] Address resolution failed:  " << ec.message() << std::endl;
                continue;
            }

            // Connect to endpoint
            asio::connect(this->socket, endpoints.begin(), ec);
            if (ec)
            {
                std::cout << "[NETWORK] Connection failed:  " << ec.message() << std::endl;
                continue;
            }

            this->is_connected = true;

            std::cout << "[NETWORK] Connected! " << std::endl;

            // Notify of connection
            this->connected_callback();

        } while (!this->is_connected);
    }

    // Read message from the socket
    void read_message(asio::error_code &ec)
    {
        NetFrame frame;

        // Read length of message
        asio::read(this->socket, asio::buffer(&frame.length, sizeof(frame.length)), ec);

        if (ec)
            return;

        // Prepare buffer for message
        frame.auto_resize();

        // Read message
        asio::read(this->socket, asio::buffer(frame.payload), ec);

        if (ec)
            return;

        message_received(frame.payload);
    }

    void message_received(std::string msg)
    {
        std::cout << "Message received: " << msg;

        this->message_received_callback(msg);
    }

    void set_keep_alive()
    {
        unsigned long timeout_milli = 10000;

        // platform-specific switch
#if defined _WIN32 || defined WIN32 || defined OS_WIN64 || defined _WIN64 || defined WIN64 || defined WINNT
        // use windows-specific time
        int32_t timeout = timeout_milli;
        setsockopt(socket.native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
        setsockopt(socket.native_handle(), SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
#else
        // assume everything else is posix
        struct timeval tv;
        tv.tv_sec = timeout_milli / 1000;
        tv.tv_usec = (timeout_milli % 1000) * 1000;
        setsockopt(socket.native_handle(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        setsockopt(socket.native_handle(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif
    }

    asio::io_context io_ctx;
    asio::ip::tcp::socket socket;

    std::string host;
    short port;

    std::thread reader_thr;
    std::thread writer_thr;
    std::thread connector_thr;

    std::atomic<bool> is_connected;
    std::atomic<bool> run = true;

    std::function<void(std::string)> message_received_callback;
    std::function<void()> connected_callback;

    TSQueue<std::string> outgoing;
};