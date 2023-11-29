/**
 * PMGPIeWorker Implementation
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#include <iostream>

#include "pmgpie_worker.hpp"
#include "pmgpie_worker_config.hpp"

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
    std::cout << "  Coordinator IP: " << this->conf.coordinator_ip.value() << std::endl;
    if (this->conf.worker_id.has_value())
        std::cout << "  Worker Node ID: " << this->conf.worker_id.value() << std::endl;
}
