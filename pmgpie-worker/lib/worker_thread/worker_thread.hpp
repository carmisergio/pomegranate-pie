/**
 * WorkerThread
 *
 * Manages one worker thread
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 30/11/2023
 */

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <optional>

#include "pi.hpp"

namespace worker_thread
{
    /**
     * Representation of a work unit
     */
    struct WorkUnit
    {
        long long start; // Starting digit
        long long n;     // Number of digits
    };

    /**
     * WorkerThread class
     */
    class WorkerThread
    {
    public:
        /**
         * Start worker thread
         */
        void start();

        /**
         * Stop worker thread
         */
        void stop();

        /**
         * Tells the thread to start computing a work unit
         *
         * @param work_unit work unit to compute
         * @throws worker_thread::WorkerBusy if thread is already computing
         */
        void submit_work_unit(WorkUnit work_unit);

    private:
        // Status vars
        std::atomic<bool> busy; // True if thread is computing
        std::atomic<bool> run;  // Variable to notify thread of when to exit

        // Thread
        std::thread thread; // Actual controlled thread
        std::mutex thread_mutex;
        std::condition_variable thread_cv;

        // New work unit to be processed
        std::optional<WorkUnit> next_work_unit;

        // Pi generator
        pi::PiGenerator pi_gen;

        void worker_function();
        void compute_new_work_unit();
    };

    /**
     * Worker is already busy exception
     */
    class WorkerBusy : public std::exception
    {
    public:
        const char *what();
    };

}
