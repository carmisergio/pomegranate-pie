/**
 * Network client class implementation
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 05/12/2023
 */

#pragma once

#include <functional>

#include "net_client_connection.hpp"
#include "worker_thread_pool_common.hpp"
#include "pmgpie_cluster.pb.h"

namespace net_client
{
    class PMGPIeClusterClient
    {
    public:
        PMGPIeClusterClient(std::string host,
                            unsigned short port,
                            std::string worker_id,
                            int n_threads,
                            std::function<void(worker_pool::WorkUnit)> submit_work_unit,
                            std::function<void()> quit) : connection(
                                                              host, port,
                                                              [this](std::string msg)
                                                              { this->con_msg_received(msg); },
                                                              [this]()
                                                              { this->con_connected(); })
        {
            this->worker_id = worker_id;

            this->submit_work_unit = submit_work_unit;
            this->quit = quit;

            // Send bootstrap RequestWorkUnit messages,
            // one for each worker thread
            pmgpie_cluster::ClientMessage msg;
            msg.set_worker_id(worker_id);
            msg.set_allocated_requestworkunit(new pmgpie_cluster::RequestWorkUnit());
            for (int i = 0; i < n_threads; i++)
                send_message(msg);
        }

        // Destructor
        ~PMGPIeClusterClient()
        {
            // Construct goodbye message
            pmgpie_cluster::ClientMessage message;
            message.set_worker_id(this->worker_id);
            message.set_allocated_goodbye(new pmgpie_cluster::GoodBye());

            send_message(message);
        }

        void publish_work_unit_result(worker_pool::WorkUnitResult result)
        {

            pmgpie_cluster::ClientMessage msg;

            msg.set_worker_id(this->worker_id);

            // Send WorkUnitResult message
            auto body = new pmgpie_cluster::WorkUnitResult();
            body->set_start(result.get_start());
            body->set_digits(result.get_digits());
            msg.set_allocated_workunitresult(body);

            send_message(msg);

            // Send RequestWorkUnit message
            msg.set_allocated_requestworkunit(new pmgpie_cluster::RequestWorkUnit());

            send_message(msg);
        }

    private:
        void send_message(pmgpie_cluster::ClientMessage msg)
        {
            this->connection.send_message(msg.SerializeAsString());
        }

        // ServerMessage received from the connection
        void message_received(pmgpie_cluster::ServerMessage msg)
        {
            if (msg.has_dispatchworkunit())
            {
                // Message is DispatchWorkUnit

                // Dispatch new work unit based on message received
                submit_work_unit(worker_pool::WorkUnit{
                    (long long)msg.dispatchworkunit().start(),
                    msg.dispatchworkunit().n_digits()});
            }
            else if (msg.has_goodbye())
            {
                std::cout << "[CLUSTER] GoodBye message received!" << std::endl;
                this->quit();
            }
        }

        // Connection callbacks
        void con_msg_received(std::string msg)
        {
            pmgpie_cluster::ServerMessage message;
            if (message.ParseFromString(msg))
                message_received(message);
        }

        void con_connected()
        {
            // Construct helo message
            auto message = std::make_shared<pmgpie_cluster::ClientMessage>();
            message->set_worker_id(this->worker_id);
            message->set_allocated_helo(new pmgpie_cluster::Helo());

            send_message(*message);
        }

        std::string worker_id;

        std::function<void(worker_pool::WorkUnit)> submit_work_unit;
        std::function<void()> quit;

        ClientConnection connection;
    };

}