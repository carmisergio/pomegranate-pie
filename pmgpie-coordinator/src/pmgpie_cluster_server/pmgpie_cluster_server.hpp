/**
 * TCP Server connection object
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#include <iostream>
#include <syncstream>

#include "tcp_server.hpp"

#include "pmgpie_cluster.pb.h"

namespace pmgpie_cluster_server
{

    class PMGPIeClusterServer
    {
    public:
        /**
         * Constructor
         */
        PMGPIeClusterServer(int port)
            : tcp_server(
                  port,
                  [](std::string msg, std::shared_ptr<tcp_server::TCPServerConnection> connection)
                  {
                      std::cout << "[CLUSTER] << Message received: " << msg << std::endl;
                  }) {}

        /**
         * Destructor
         */
        ~PMGPIeClusterServer()
        {
        }

    private:
        tcp_server::TCPServer tcp_server;
    };
}