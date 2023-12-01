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

void worker_thread::WorkerThread::start()
{
    // Start thread
    this->run = true;
    this->thread = std::thread(&worker_thread::WorkerThread::worker_function, this);
}

void worker_thread::WorkerThread::stop()
{
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
    std::cout << "Exiting thread" << std::endl;
}

/**
 * Compute the work unit in next_work_unit
 */
void worker_thread::WorkerThread::compute_new_work_unit()
{
    if (this->next_work_unit.has_value())
    {
        std::cout << "Computing new work unit" << std::endl;

        // Generate digits of pi
        std::string digits;
        try
        {
            digits = this->pi_gen.generate_pi_digits(
                this->next_work_unit.value().start,
                this->next_work_unit.value().n);
        }
        catch (pi::ExecutionInterrupted _)
        {
            // Swallow exception
        }

        // There is no new work unit
        this->next_work_unit.reset();

        std::cout << "New digits: " << digits << std::endl;
    }
}

const char *worker_thread::WorkerBusy::what()
{
    return "Cannot continue execution";
}
