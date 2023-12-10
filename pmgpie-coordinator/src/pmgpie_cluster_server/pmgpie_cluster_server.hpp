/**
 * TCP Server connection object
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#pragma once

#include <iostream>
#include <syncstream>
#include <memory>

#include "tcp_server.hpp"

#include "pmgpie_cluster.pb.h"
#include "work_unit_manager.hpp"
#include "stats.hpp"

namespace pmgpie_cluster_server
{

    class PMGPIeClusterServer
    {
    public:
        /**
         * Constructor
         */
        PMGPIeClusterServer(
            int port,
            std::shared_ptr<work_unit_manager::WorkUnitManager> work_unit_manager,
            std::shared_ptr<PMGPIeClusterStats> stats)
            : work_unit_manager(work_unit_manager),
              stats(stats),
              tcp_server(
                  port,
                  [this](std::string msg, std::shared_ptr<tcp_server::TCPServerConnection> connection)
                  { this->con_received_message(msg, connection); },
                  [this](std::shared_ptr<tcp_server::TCPServerConnection> connection)
                  { this->con_disconnected(connection); })
        {
            std::osyncstream(std::cout) << "[CLUSTER] Listening on port  " << port << std::endl;
        }

        /**
         * Destructor
         */
        ~PMGPIeClusterServer()
        {
            // Send Goodbye message to all cluster clients
            pmgpie_cluster::ServerMessage reply;
            reply.set_allocated_goodbye(new pmgpie_cluster::GoodBye);
            send_all(reply);
        }

    private:
        void send_message(pmgpie_cluster::ServerMessage msg, std::shared_ptr<tcp_server::TCPServerConnection> connection)
        {
            connection->send_message(msg.SerializeAsString());
        }

        void send_all(pmgpie_cluster::ServerMessage msg)
        {
            this->tcp_server.send_all(msg.SerializeAsString());
        }

        // Parse message arriving as strings from the TCPServer
        void con_received_message(std::string msg, std::shared_ptr<tcp_server::TCPServerConnection> connection)
        {
            pmgpie_cluster::ClientMessage message;

            // Parse protobuf message and pass it on to the handler
            if (message.ParseFromString(msg))
                message_received(message, connection);
        }

        // Connection disconnected
        void con_disconnected(std::shared_ptr<tcp_server::TCPServerConnection> connection)
        {
            std::string worker_id = connection->worker_id;

            std::osyncstream(std::cout) << "[CLUSTER] Lost conneciton to worker: " << worker_id << std::endl;

            // Check that the client had the time to identify itself
            if (worker_id != "")
            {
                this->work_unit_manager->mark_orphaned(connection->worker_id);

                // Count client leaving
                this->n_clients--;
                stats->worker_nodes = n_clients;
            }
        }

        // ClientMessage received from any connection
        void message_received(pmgpie_cluster::ClientMessage msg, std::shared_ptr<tcp_server::TCPServerConnection> connection)
        {
            // Check worker id field is present
            if (msg.worker_id() == "")
                return;

            // Remember worker_id on connection
            connection->worker_id = msg.worker_id();

            // Handle each message type
            if (msg.has_helo())
                handle_helo_message(msg.worker_id());
            else if (msg.has_goodbye())
                handle_goodbye_message(msg.worker_id());
            else if (msg.has_workunitresult())
                handle_workunitresult_message(msg.worker_id(), msg.workunitresult());
            else if (msg.has_requestworkunit())
                handle_requestworkunit_message(msg.worker_id(), connection);
        }

        void handle_helo_message(std::string worker_id)
        {
            // std::osyncstream(std::cout) << "[CLUSTER][" << worker_id << "]"
            //                             << " Joined!" << std::endl;

            this->work_unit_manager->mark_owned(worker_id);

            // Count client joining
            this->n_clients++;
            stats->worker_nodes = n_clients;
        }

        void handle_goodbye_message(std::string worker_id)
        {
            std::osyncstream(std::cout) << "[CLUSTER][" << worker_id << "]"
                                        << " Goodbye!" << std::endl;

            // Mark this worker's owned work units as disowned
            this->work_unit_manager->mark_disowned(worker_id);
        }

        void handle_workunitresult_message(std::string worker_id, pmgpie_cluster::WorkUnitResult workunitresult)
        {
            // std::osyncstream(std::cout) << "WorkUnitResult message received from " << worker_id << std::endl;

            // std::osyncstream(std::cout) << "\tdigits: " << workunitresult.digits().substr(0, 10) << "... size: " << workunitresult.digits().length() << std::endl;

            std::osyncstream(std::cout) << "[CLUSTER][" << worker_id << "]"
                                        << " Work unit result (start = " << workunitresult.start() << ", n = "
                                        << workunitresult.digits().size() << ")" << std::endl;

            // Submit result to work unit manager
            this->work_unit_manager->submit_result(workunitresult.digits(), workunitresult.start());
        }

        void handle_requestworkunit_message(std::string worker_id, std::shared_ptr<tcp_server::TCPServerConnection> connection)
        {
            // Get work unit from work unit manager
            auto work_unit =
                this->work_unit_manager->assign_work_unit(worker_id);

            pmgpie_cluster::ServerMessage reply;
            pmgpie_cluster::DispatchWorkUnit *dwu = new pmgpie_cluster::DispatchWorkUnit;
            dwu->set_start(work_unit.start);
            dwu->set_n_digits(work_unit.n_digits);
            reply.set_allocated_dispatchworkunit(dwu);

            send_message(reply, connection);

            std::osyncstream(std::cout) << "[CLUSTER][" << worker_id << "]"
                                        << " Dispatched work unit: (start = " << work_unit.start << ", n = "
                                        << work_unit.n_digits << ")" << std::endl;
        }

        tcp_server::TCPServer tcp_server;
        std::shared_ptr<work_unit_manager::WorkUnitManager> work_unit_manager;

        std::atomic<int> n_clients{0};
        std::shared_ptr<PMGPIeClusterStats> stats;
    };
}