/**
 * PMGPIeWorker Implementation
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#include <iostream>
#include <chrono>

#include "pmgpie_worker.hpp"
#include "pmgpie_worker_config.hpp"
#include "worker_thread.hpp"

/**
 * Constructor
 */
PMGPIeWorker::PMGPIeWorker(config::pmgpie_worker_config conf)
{
    // Save configuration
    this->conf = conf;
}

/**
 * Main entry point
 */
void PMGPIeWorker::run()
{
    std::cout << "Hello from PMGPIe Worker class!" << std::endl;
    std::cout << "  Coordinator Host: " << this->conf.coordinator_host.value() << std::endl;
    if (this->conf.worker_id.has_value())
        std::cout << "  Worker Node ID: " << this->conf.worker_id.value() << std::endl;
    if (this->conf.threads.has_value())
        std::cout << "  Threads: " << this->conf.threads.value() << std::endl;

    worker_thread::WorkerThread wt;
    wt.start();

    for (int i = 0; i < 30; i++)
    {
        if (i == 5)
            wt.submit_work_unit(worker_thread::WorkUnit{10000, 1000});

        if (i == 25)
            wt.submit_work_unit(worker_thread::WorkUnit{1000000, 100});

        std::cout << "Looping..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    wt.stop();
}
