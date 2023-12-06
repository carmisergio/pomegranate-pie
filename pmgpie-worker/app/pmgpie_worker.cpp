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

void result_callback(worker_pool::WorkUnitResult result)
{
    std::cout << "## Thread result:" << std::endl;
    std::cout << "Start: " << result.get_start() << std::endl;
    std::cout << "Digits: " << result.get_digits() << std::endl;
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

    std::cout << "[MAIN] This worker ID: " << this->worker_id << std::endl;

    // Start all worker threads
    std::cout << "[MAIN] Started!" << std::endl;

    this->running = true;

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // this->worker_thread_pool.submit_work_unit(worker_pool::WorkUnit{0, 10000});

    // int i = 0;
    // for (int j = 0; j < 10; j++)
    // {
    //     this->worker_thread_pool.submit_work_unit(worker_pool::WorkUnit{i, 1000});
    //     i += 1000;
    // }

    // std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    //     i = 0;
    //     for (int j = 0; j < 10; j++)
    //     {
    //         this->worker_thread_pool.submit_work_unit(worker_pool::WorkUnit{i, 1000});
    //         i += 1000;
    //     }

    //     std::this_thread::sleep_for(std::chrono::milliseconds(30000));

    // for (int j = 0; j < this->worker_thread_pool.n_threads; j++)
    // {
    //     this->worker_thread_pool.submit_work_unit(worker_pool::WorkUnit{0, 3000});
    //     // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    while (this->running.load())
    {
        for (int j = 0; j < this->worker_thread_pool.n_threads; j++)
        {
            this->worker_thread_pool.submit_work_unit(worker_pool::WorkUnit{0, 1000});
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    std::cout << "Exiting..." << std::endl;
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
    // std::cout << "CTRL-C Handler" << std::endl;

    // Stop main thread
    this->running = false;
}