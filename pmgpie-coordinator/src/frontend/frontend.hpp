/**
 * WS Server for the frontend
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 10/12/2023
 */

#pragma once

#include <iostream>
#include <thread>
#include <memory>
#include <syncstream>

#include "seasocks/PageHandler.h"
#include "seasocks/Logger.h"
#include "seasocks/Server.h"
#include "seasocks/WebSocket.h"
#include "seasocks/StringUtil.h"
#include "json.hpp"

#include "stats.hpp"

namespace frontend
{
    using namespace seasocks;

    class CustomLogger : public Logger
    {
        virtual void log(Level level, const char *message)
        {
            if (level == Level::Error || level == Level::Severe)
                std::cout << message << std::endl;
        };
    };

    struct Handler : WebSocket::Handler
    {
        std::set<WebSocket *> _cons;

        std::shared_ptr<PMGPIeClusterStats> stats;

        Handler(std::shared_ptr<PMGPIeClusterStats> stats)
        {
            this->stats = stats;
        }

        void onConnect(WebSocket *con) override
        {
            _cons.insert(con);
        }
        void onDisconnect(WebSocket *con) override
        {
            _cons.erase(con);
        }

        void onData(WebSocket *con, const char *data) override
        {
            // Parse json data
            nlohmann::json j;
            try
            {
                j = nlohmann::json::parse(data);
            }
            catch (std::exception &e)
            {
                return;
            }

            // Handle all different messages
            if (j["type"] == "request_stats")
                send_stats(con);
            if (j["type"] == "request_digits")
                send_digits(con);
        }

        void send_stats(WebSocket *con)
        {
            nlohmann::json j;

            // Construct message
            j["type"] = "publish_stats";
            j["body"] = {
                {"hex_digits", stats->hex_digits_generated},
                {"throughput", stats->throughput},
                {"worker_nodes", stats->worker_nodes}};

            con->send(j.dump());
        }

        void send_digits(WebSocket *con)
        {
            nlohmann::json j;

            // Construct message
            j["type"] = "publish_digits";
            j["body"] = {
                {"digits", stats->last_100_digits}};

            con->send(j.dump());
        }
    };

    // Frontend Server Class
    class FrontendServer
    {
    public:
        FrontendServer(std::shared_ptr<PMGPIeClusterStats> stats)
            : stats(stats),
              server(std::make_shared<CustomLogger>())
        {

            // Add WS handler
            server.addWebSocketHandler("/stats", std::make_shared<Handler>(stats));

            // Run server thread
            ws_server_thr = std::make_shared<std::thread>(&FrontendServer::run_ws_server, this);

            std::osyncstream(std::cout) << "[FRONTEND] Available on port 8080" << std::endl;
        }

        ~FrontendServer()
        {
            // Stop server
            server.terminate();

            if (this->ws_server_thr->joinable())
                this->ws_server_thr->join();
        }

    private:
        void run_ws_server()
        {
            server.serve("web", 8080);
        }

        void publish_stats_all()
        {
        }

        std::shared_ptr<PMGPIeClusterStats> stats;

        std::shared_ptr<std::thread> ws_server_thr;

        // Seasocks server
        Server server;
    };
}
