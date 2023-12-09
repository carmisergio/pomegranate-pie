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
#include <thread>
#include <syncstream>

#define UUID_SYSTEM_GENERATOR
#include "uuid.hpp"

#include "pmgpie_worker.hpp"

#define MAX_THREADS 999

/**
 * Generate a new UUID
 */
std::string generate_uuid()
{
    uuids::uuid const id = uuids::uuid_system_generator{}();
    return uuids::to_string(id);
}

/**
 * Constructor
 */
PMGPIeWorker::PMGPIeWorker(config::pmgpie_worker_config conf)
    : worker_id(generate_uuid()),
      worker_thread_pool(get_worker_thread_n(conf),
                         [this](worker_pool::WorkUnitResult result)
                         { this->cluster_client.publish_work_unit_result(result); }),
      cluster_client(
          conf.coordinator_host.value(),
          3141,
          this->worker_id,
          this->worker_thread_pool.n_threads,
          [this](worker_pool::WorkUnit work_unit)
          { this->worker_thread_pool.submit_work_unit(work_unit); },
          [this]()
          {
              this->quit();
          })
{
    // Save configuration
    this->conf = conf;
}

/**
 * Main entry point
 */
void PMGPIeWorker::run()
{

    // Setup CTRL-C events
    this->setup_ctrlc_handler();

    std::osyncstream(std::cout) << "[MAIN] This worker ID: " << this->worker_id << std::endl;

    // Start all worker threads
    std::osyncstream(std::osyncstream(std::cout)) << "[MAIN] Started!" << std::endl;

    this->running = true;

    while (this->running.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::osyncstream(std::cout) << "Exiting..." << std::endl;
}

/**
 * Get n. of worker threads from config if given, otherwise
 * from number of hardware threads available
 */
int PMGPIeWorker::get_worker_thread_n(config::pmgpie_worker_config conf)
{
    // Check if thread number option was provided
    if (conf.threads.has_value())
        return conf.threads.value();

    // Get number of available threads on this machine
    return std::thread::hardware_concurrency();
}

/**
 * Sets up the CTRL C handler
 */
void PMGPIeWorker::setup_ctrlc_handler()
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
void PMGPIeWorker::quit()
{
    // Stop main thread
    this->running = false;
}