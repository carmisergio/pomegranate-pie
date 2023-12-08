/**
 * PMGPIeCoordinator Implementation
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 07/12/2023
 */

#include <iostream>
#include <thread>
#include <chrono>

#include "pmgpie_coordinator.hpp"

/**
 * Constructor
 */
PMGPIeCoordinator::PMGPIeCoordinator(config::pmgpie_coordinator_config conf)
    : pmgpie_cluster_server(conf.port.value()),
      conf(conf)
{
}

/**
 * Main entry point
 */
void PMGPIeCoordinator::run()
{

    std::cout << "PMGPIe Coordinator started!" << std::endl;

    // Run
    this->running = true;

    // Setup CTRL-C events
    this->setup_ctrlc_handler();

    while (this->running.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // this->tcp_server.send_all("Goodbye!");

    std::cout << "Exiting..." << std::endl;
}

/**
 * Sets up the CTRL C handler
 */
void PMGPIeCoordinator::setup_ctrlc_handler()
{
    // Register CTRL-C handler
    unsigned int handler_id = CtrlCLibrary::SetCtrlCHandler(
        [this](enum CtrlCLibrary::CtrlSignal event) -> bool
        {
            if (event == CtrlCLibrary::kCtrlCSignal)
                this->quit();

            return true;
        });
}

/**
 * Handle CTRL-C events
 */
void PMGPIeCoordinator::quit()
{
    // Stop main thread
    this->running = false;
}