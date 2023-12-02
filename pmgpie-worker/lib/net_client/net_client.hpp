/**
 * Network client for PMGPIe Worker
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 02/12/2023
 */

#pragma once

#include <functional>
#include <optional>

#include "asio.hpp"
#include "worker_thread.hpp"
#include "net_message.hpp"

namespace net
{
    /**
     * Network client class
     */
    class NetworkClient
    {
    public:
        /**
         * Constructor
         *
         * @param coordinator_host hostname of coordinator to conenct to
         */
        NetworkClient(std::string coordinator_host);

        /**
         * Run the network client (blocking)
         */
        void run();

        /**
         * Set function to be called to dispatch a work unit on a thread
         *
         * @param callback callback function
         */
        void set_dispatch_work_unit_callback(std::function<void(worker_thread::WorkUnit, int)> callback);

    private:
        // Config vars
        std::string coordinator_host;
        std::string port;

        // ASIO stuff
        std::shared_ptr<asio::io_service> io_service;
        std::shared_ptr<asio::ip::tcp::socket> socket;

        // Connection status
        bool connected;

        // Dispatch work unit callback function
        std::optional<std::function<void(worker_thread::WorkUnit, int)>> dispatch_work_unit_callback;

        // Connection management
        void do_connect();
        net::Message read_message();
        net::MessageHeader read_header();
        void read_body(size_t length, std::vector<uint8_t> &body);

        // Handle received messages
        void handle_message(net::Message message);
        void handle_dispatch_work_unit_message(net::Message message);

        void dispatch_work_unit(worker_thread::WorkUnit work_unit, int thread);
    };
};
