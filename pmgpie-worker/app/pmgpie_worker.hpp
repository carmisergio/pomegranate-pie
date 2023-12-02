/**
 * PMGPIeWorker Class
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#pragma once

#include <vector>

#include "pmgpie_worker_config.hpp"
#include "worker_thread.hpp"
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
    // Worker threads
    std::vector<std::shared_ptr<worker_thread::WorkerThread>> worker_threads;

    // CTRL-C Handler
    void setup_ctrlc_handler();
    void ctrlc_handler();

    // Worker threads funcitons
    void spawn_threads();
    void start_all_threads();
    void stop_all_threads();
    int get_worker_thread_n();
    void dispatch_work_unit(worker_thread::WorkUnit work_unit, int thread);

    bool running;
};
