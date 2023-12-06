/**
 * WorkerThread implementation
 *
 * Manages one worker thread
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 30/11/2023
 */

#include <iostream>
#include <syncstream>

#include "worker_thread.hpp"

worker_pool::WorkerThread::WorkerThread(int thread_id,
                                        std::function<void(WorkUnitResult)> submit_result_callback)
{
    // Constructor
    this->thread_id = thread_id;
    this->result_callback = submit_result_callback;

    this->busy = false;

    // Start thread
    start();
}

worker_pool::WorkerThread::~WorkerThread()
{
    // std::osyncstream(std::cout) << "[THREAD " << this->thread_id << "] Exiting..." << std::endl;

    // Check if thread is already not running
    if (!this->run)
        return;

    // Tell any running calculation to stop
    this->pi_gen.interrupt();

    // Tell thread to stop running
    this->run = false;

    // Wake thread
    {
        std::unique_lock lock(this->thread_mutex);
        this->thread_cv.notify_one();
    }

    // Wait for thread to exit
    this->thread.join();
}

void worker_pool::WorkerThread::start()
{
    // Start thread
    this->run = true;
    this->thread = std::thread(&worker_pool::WorkerThread::worker_function, this);

    // std::osyncstream(std::cout) << "[THREAD " << this->thread_id << "] Ready!" << std::endl;
}

void worker_pool::WorkerThread::submit_work_unit(WorkUnit work_unit)
{
    // Check if thread is already busy
    if (this->busy.load())
        throw worker_pool::WorkerBusy();

    this->busy = true; // Set busy flag

    // Acquire lock
    std::unique_lock lock(this->thread_mutex);

    // Pass work unti to worker thread
    this->next_work_unit = work_unit;

    // Notify worker thread
    this->thread_cv.notify_one();
}

bool worker_pool::WorkerThread::is_busy()
{
    return this->busy;
}

/**
 * Function which is run inside the thread
 */
void worker_pool::WorkerThread::worker_function()
{
    while (this->run)
    {
        // Acquire lock
        std::unique_lock lock(this->thread_mutex);

        // Wait for CV
        this->thread_cv.wait(lock);

        // See if there is a new work unit to process
        try
        {
            this->compute_new_work_unit();
        }
        catch (std::exception _)
        {
            // Swallow ExecutionInterrupted errors
        };

        this->busy = false;
    }
    // std::osyncstream(std::cout) << "[WORKER THREAD " << this->thread_id << "] Stopped" << std::endl;
}

/**
 * Compute the work unit in next_work_unit
 */
void worker_pool::WorkerThread::compute_new_work_unit()
{
    if (this->next_work_unit.has_value())
    {
        std::osyncstream(std::cout) << "[THREAD " << this->thread_id << "] Computing work unit (start = "
                                    << next_work_unit.value().start << ", n = "
                                    << next_work_unit.value().n << ")" << std::endl;

        // Generate digits of pi
        std::string digits = this->pi_gen.generate_pi_digits(
            this->next_work_unit.value().start,
            this->next_work_unit.value().n);

        worker_pool::WorkUnitResult result(digits, this->next_work_unit.value().start);

        std::osyncstream(std::cout) << "[THREAD " << this->thread_id << "] Work unit done!" << std::endl;

        // Submit result to main thread
        this->submit_result(result);

        // There is no new work unit
        this->next_work_unit.reset();
    }
}

/**
 * Submit result to callback
 *
 * @param result work unit result
 */
void worker_pool::WorkerThread::submit_result(worker_pool::WorkUnitResult result)
{
    // Check that we have the callback set
    this->result_callback(result);
}
