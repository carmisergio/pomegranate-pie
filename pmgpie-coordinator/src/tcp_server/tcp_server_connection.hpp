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
#include <string>

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
            std::function<void(std::string, std::shared_ptr<TCPServerConnection> connection)> message_received_callback,
            std::function<void(std::shared_ptr<TCPServerConnection> connection)> disconnected_callback)
            : socket(socket),
              add_to_con_removal_list(add_to_con_removal_list),
              message_received_callback(message_received_callback),
              disconnected_callback(disconnected_callback)
        {

            // auto ptr = shared_from_this();
            std::cout << "TCPServerConnection constructor" << std::endl;
            // Set TCP keepalive
            int val = 1;
            setsockopt(this->socket->native_handle(), SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
            val = 10;
            setsockopt(this->socket->native_handle(), SOL_TCP, TCP_KEEPIDLE, &val, sizeof(val));
            val = 5;
            setsockopt(this->socket->native_handle(), SOL_TCP, TCP_KEEPCNT, &val, sizeof(val));
            val = 1;
            setsockopt(this->socket->native_handle(), SOL_TCP, TCP_KEEPINTVL, &val, sizeof(val));
        };

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

        /**
         * Starts the connection threads
         */
        void start()
        {
            // Start reader and writer thread
            this->reader_thr = std::make_unique<std::thread>(&TCPServerConnection::reader, this);
            this->writer_thr = std::make_unique<std::thread>(&TCPServerConnection::writer, this);
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

            try
            {
                this->socket->shutdown(asio::ip::tcp::socket::shutdown_receive);
            }
            catch (std::system_error &e)
            {
            }

            if (this->reader_thr->joinable())
                this->reader_thr->join();

            this->outgoing.push("");
            if (this->writer_thr->joinable())
                this->writer_thr->join();
        }

        std::string worker_id; // To be used externally

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

                    // Call disconnected callbackdisconnected_calback
                    this->disconnected_callback(shared_from_this());

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

                        // Call disconnected callbackdisconnected_calback
                        this->disconnected_callback(shared_from_this());

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
        std::function<void(std::shared_ptr<TCPServerConnection>)> disconnected_callback;

        std::unique_ptr<std::thread> reader_thr;
        std::unique_ptr<std::thread> writer_thr;

        TSQueue<std::string> outgoing;
    };
}