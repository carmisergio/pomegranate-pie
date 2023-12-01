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

#include "pmgpie_worker.hpp"

#define MAX_THREADS 999

void result_callback(worker_thread::WorkUnitResult result, int thread_id)
{
    std::cout << "## Thread " << thread_id << " result:" << std::endl;
    if (result.success())
    {
        std::cout << "Start: " << result.get_start() << std::endl;
        std::cout << "Digits: " << result.get_digits() << std::endl;
    }

    else
        std::cout << "Unsuccesful!" << std::endl;
}

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

    // Setup CTRL-C events
    this->setup_ctrlc_handler();

    std::cout << "Hello from PMGPIe Worker class!" << std::endl;
    std::cout << "  Coordinator Host: " << this->conf.coordinator_host.value() << std::endl;
    if (this->conf.worker_id.has_value())
        std::cout << "  Worker Node ID: " << this->conf.worker_id.value() << std::endl;
    if (this->conf.threads.has_value())
        std::cout << "  Threads: " << this->conf.threads.value() << std::endl;

    // Spawn worker threads
    this->spawn_threads();

    // Start all worker threads
    this->start_all_threads();

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    int i = 0;
    for (auto &thread : this->worker_threads)
    {
        thread->submit_work_unit(worker_thread::WorkUnit{i, 1000});
        i += 1000;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    for (auto &thread : this->worker_threads)
    {
        thread->submit_work_unit(worker_thread::WorkUnit{i, 1000});
        i += 1000;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(30000));

    // Stop and wait for all worker threads
    this->stop_all_threads();
}

/**
 * Create correct number of worker threads
 */
void PMGPIeWorker::spawn_threads()
{
    // Get number of threads to spwan
    int n_threads = this->get_worker_thread_n();

    std::cout << "Spawning " << n_threads << " threads..." << std::endl;

    // Spawn threads
    for (int i = 0; i < n_threads; i++)
    {
        // Create new thread
        auto new_thread = std::make_shared<worker_thread::WorkerThread>(i);

        // Set result callback for this thread
        new_thread->set_result_callback([i](worker_thread::WorkUnitResult result)
                                        { result_callback(result, i); });

        // Keep track of new thread
        this->worker_threads.push_back(new_thread);
    }
}

/**
 * Get n. of worker threads from config if given, otherwise
 * from number of hardware threads available
 */
int PMGPIeWorker::get_worker_thread_n()
{
    // Check if thread number option was provided
    if (this->conf.threads.has_value())
        return this->conf.threads.value();

    // Get number of available threads on this machine
    return std::thread::hardware_concurrency();
}

/**
 * Start all worker threads
 */
void PMGPIeWorker::start_all_threads()
{
    for (auto &thread : this->worker_threads)
        thread->start();
}

/**
 * Stop all worker threads
 */
void PMGPIeWorker::stop_all_threads()
{
    for (auto &thread : this->worker_threads)
        thread->stop();
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
                this->ctrlc_handler();

            return true;
        });
}

/**
 * Handle CTRL-C events
 */
void PMGPIeWorker::ctrlc_handler()
{
    std::cout << "CTRL-C Handler" << std::endl;

    // Don't propagate error
    this->stop_all_threads();
}