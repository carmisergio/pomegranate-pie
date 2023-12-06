/**
 * PMGPIeWorker Class
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#pragma once

#include <atomic>
#include <string>

#include "pmgpie_worker_config.hpp"
#include "worker_thread_pool.hpp"
#include "ctrl_c.hpp"
#include "net_client.hpp"

/**
 * PMGPIeWorker Class
 *
 * Main class
 */
class PMGPIeWorker
{

public:
    PMGPIeWorker(config::pmgpie_worker_config conf);
    void run();

private:
    config::pmgpie_worker_config conf; // Configuration
                                       //
                                       //

    std::string worker_id;

    // Worker thread pool
    worker_pool::WorkerThreadPool worker_thread_pool;

    // PMGPIe Cluster connection
    net_client::PMGPIeClusterClient cluster_client;

    // CTRL-C Handler
    void setup_ctrlc_handler();
    void quit();

    int get_worker_thread_n(config::pmgpie_worker_config conf);

    std::atomic<bool> running;
};
