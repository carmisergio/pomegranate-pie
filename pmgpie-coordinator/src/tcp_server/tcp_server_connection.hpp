/**
 * TCP Server connection object
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#pragma once

// #include <iostream>
#include <memory>
#include <thread>

#include "asio.hpp"
#include "tsqueue.hpp"

#include "tcp_server_frame.hpp"

namespace tcp_server
{
    class TCPServerConnection : public std::enable_shared_from_this<TCPServerConnection>
    {
    public:
        TCPServerConnection(
            std::shared_ptr<asio::ip::tcp::socket> socket,
            std::function<void(std::shared_ptr<TCPServerConnection>)> add_to_con_removal_list,
            std::function<void(std::string, std::shared_ptr<TCPServerConnection> connection)> message_received_callback)
            : socket(socket),
              add_to_con_removal_list(add_to_con_removal_list),
              message_received_callback(message_received_callback),
              reader_thr(&TCPServerConnection::reader, this),
              writer_thr(&TCPServerConnection::writer, this){};

        bool is_running()
        {
            return this->is_connected.load();
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

        ~TCPServerConnection()
        {
            this->in_destructor = true;

            if (this->is_connected)
            {
                // Wait for messages to be sent
                // TODO possible condition where the socket disconnects in this procedure
                while (!this->outgoing.empty())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }

            this->socket->shutdown(asio::ip::tcp::socket::shutdown_receive);

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

            while (this->is_connected.load() && !this->in_destructor.load())
            {
                read_message(ec);

                if (ec)
                {
                    // std::cout << "Error on read: " << ec.message() << std::endl;

                    this->is_connected = false;

                    if (!this->in_destructor.load())
                        add_to_con_removal_list(shared_from_this());
                }
            }
        }

        void writer()
        {
            asio::error_code ec;
            std::string msg;

            while (this->is_connected.load() && !this->in_destructor.load())
            {
                msg = this->outgoing.pop(); // Get message to send

                if (msg != "")
                {
                    asio::write(*this->socket, asio::buffer(tcp_server::NetFrame(msg).as_bytes()), ec);

                    if (ec)
                    {
                        // std::cout << "Error on write: " << ec.message() << std::endl;

                        this->is_connected = false;

                        if (!this->in_destructor.load())
                            add_to_con_removal_list(shared_from_this());
                    }
                }
            }
        }

        // Read message from the socket
        void read_message(asio::error_code &ec)
        {
            tcp_server::NetFrame frame;

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

        std::atomic<bool> is_connected{true};
        std::atomic<bool> in_destructor{false};

        // Callbacks
        std::function<void(std::shared_ptr<TCPServerConnection>)> add_to_con_removal_list;
        std::function<void(std::string, std::shared_ptr<TCPServerConnection>)> message_received_callback;

        std::thread reader_thr;
        std::thread writer_thr;

        TSQueue<std::string> outgoing;
    };
}