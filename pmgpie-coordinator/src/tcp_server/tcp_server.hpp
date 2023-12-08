/**
 * Tcp Server object
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#include <functional>
#include <thread>
#include <memory>
#include <list>
#include <atomic>

#include "asio.hpp"

#include "tcp_server_connection.hpp"

namespace tcp_server
{
    class TCPServer
    {
    public:
        /**
         * Constructor
         */
        TCPServer(
            int port,
            std::function<void(std::string, std::shared_ptr<tcp_server::TCPServerConnection>)> message_received_callback)
            : acceptor(this->io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
              message_received_callback(message_received_callback),
              acceptor_thread(&TCPServer::acceptor_function, this),
              con_remover_thread(&TCPServer::con_remover, this){};

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

        /**
         * Destructor
         */
        ~TCPServer()
        {
            // Close acceptor

            this->run_acceptor = false;

            int shutdown_status = shutdown(this->acceptor.native_handle(), SHUT_RDWR);

            // Wait for accceptor thread to exit
            if (this->acceptor_thread.joinable())
                this->acceptor_thread.join();

            // Deconstruct all connections
            for (auto con : this->active_connections)
            {
                this->con_removal_queue.push(con);
            }

            this->run_con_remover = false;

            this->con_removal_queue.push(NULL);

            // Wait for connection remover thread to exit
            if (this->con_remover_thread.joinable())
                this->con_remover_thread.join();
        }

    private:
        void acceptor_function()
        {
            while (this->run_acceptor.load())
            {

                // Create new socket
                std::shared_ptr<asio::ip::tcp::socket> socket = std::make_shared<asio::ip::tcp::socket>(this->io_ctx);

                // Accept new connection and bind it to the previously create socket
                try
                {
                    this->acceptor.accept(*socket);
                }
                catch (asio::system_error &e)
                {
                    continue;
                }

                std::cout << "Received connection from " << socket->remote_endpoint().address() << std::endl;

                // Create connection around this socket
                this->active_connections.push_back(std::make_shared<TCPServerConnection>(
                    socket,
                    [this](std::shared_ptr<TCPServerConnection> to_remove)
                    { this->con_removal_queue.push(to_remove); },
                    this->message_received_callback));
            }
        }

        // Function responsible for deconstructing disconnected connections
        void con_remover()
        {
            while (this->run_con_remover.load())
            {
                do
                {
                    std::shared_ptr<TCPServerConnection> to_remove = this->con_removal_queue.pop();

                    if (to_remove != NULL)
                        this->active_connections.remove(to_remove);

                } while (!this->con_removal_queue.empty());
            }
        }

        asio::io_context io_ctx;
        asio::ip::tcp::acceptor acceptor;

        std::atomic<bool> run_con_remover{true};
        std::atomic<bool> run_acceptor{true};

        std::thread acceptor_thread;
        std::thread con_remover_thread;

        TSQueue<std::shared_ptr<TCPServerConnection>> con_removal_queue;

        std::list<std::shared_ptr<tcp_server::TCPServerConnection>> active_connections;

        // Callbacks
        std::function<void(std::string, std::shared_ptr<TCPServerConnection>)> message_received_callback;
    };
}