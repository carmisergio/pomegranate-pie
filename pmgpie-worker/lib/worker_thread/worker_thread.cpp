/**
 * WorkerThread implementation
 *
 * Manages one worker thread
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 30/11/2023
 */

#include <iostream>

#include "worker_thread.hpp"

worker_thread::WorkerThread::WorkerThread(int thread_id)
{
    // Constructor
    this->thread_id = thread_id;
}

void worker_thread::WorkerThread::start()
{
    // Start thread
    this->run = true;
    this->thread = std::thread(&worker_thread::WorkerThread::worker_function, this);

    std::cout << "[WORKER THREAD " << this->thread_id << "] Ready!" << std::endl;
}

void worker_thread::WorkerThread::stop()
{
    std::cout << "[WORKER THREAD " << this->thread_id << "] Exiting..." << std::endl;

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
void worker_thread::WorkerThread::set_result_callback(std::function<void(worker_thread::WorkUnitResult)> callback)
{
    // Set callback
    this->result_callback = callback;
}

void worker_thread::WorkerThread::submit_work_unit(WorkUnit work_unit)
{
    // Check if thread is already busy
    if (this->busy.load())
        throw worker_thread::WorkerBusy();

    this->busy = true; // Set busy flag

    // Acquire lock
    std::unique_lock lock(this->thread_mutex);

    // Pass work unti to worker thread
    this->next_work_unit = work_unit;

    // Notify worker thread
    this->thread_cv.notify_one();
}

/**
 * Function which is run inside the thread
 */
void worker_thread::WorkerThread::worker_function()
{
    while (this->run)
    {
        // Acquire lock
        std::unique_lock lock(this->thread_mutex);

        // Wait for CV
        this->thread_cv.wait(lock);

        // See if there is a new work unit to process
        this->compute_new_work_unit();

        this->busy = false;
    }
    std::cout << "[WORKER THREAD " << this->thread_id << "] Stopped" << std::endl;
}

/**
 * Compute the work unit in next_work_unit
 */
void worker_thread::WorkerThread::compute_new_work_unit()
{
    if (this->next_work_unit.has_value())
    {
        std::cout << "Computing new work unit on thread: " << this->thread_id << std::endl;

        // Create result object
        worker_thread::WorkUnitResult result;

        // Generate digits of pi
        try
        {
            std::string digits = this->pi_gen.generate_pi_digits(
                this->next_work_unit.value().start,
                this->next_work_unit.value().n);

            result = worker_thread::WorkUnitResult::succesful(digits, this->next_work_unit.value().start);
        }
        catch (pi::ExecutionInterrupted _)
        {
            // Swallow exception
            result = worker_thread::WorkUnitResult::unsuccesful();
        }

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
void worker_thread::WorkerThread::submit_result(worker_thread::WorkUnitResult result)
{
    // Check that we have the callback set
    if (this->result_callback.has_value())
        this->result_callback.value()(result);
    else
        throw std::runtime_error("Callback not set!");
}

/**
 * Constructs a WorkUnitResult for a succesful work unit
 */
worker_thread::WorkUnitResult worker_thread::WorkUnitResult::succesful(std::string digits, long long start)
{
    worker_thread::WorkUnitResult result;

    result.was_succesful = true;
    result.digits = digits;
    result.start = start;

    return result;
}

/**
 * Constructs a WorkUnitResult for an unsuccesful work unit
 */
worker_thread::WorkUnitResult worker_thread::WorkUnitResult::unsuccesful()
{
    worker_thread::WorkUnitResult result;

    result.was_succesful = false;

    return result;
}

/**
 * Gets success
 *
 * @returns true if work unit was succesful
 */
bool worker_thread::WorkUnitResult::success()
{
    return this->was_succesful;
}

/**
 * Gets digits
 *
 * @returns digits
 */
std::string worker_thread::WorkUnitResult::get_digits()
{
    return this->digits;
}

/**
 * Gets start point of this work unit
 *
 * @returns work unit start
 */
long long worker_thread::WorkUnitResult::get_start()
{
    return this->start;
}

const char *worker_thread::WorkerBusy::what()
{
    return "Cannot continue execution";
}
