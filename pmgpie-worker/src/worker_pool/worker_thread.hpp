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
#include <functional>
#include <syncstream>
#include <iostream>

#include "pi.hpp"
#include "worker_thread_pool_common.hpp"

namespace worker_pool
{

    /**
     * WorkerThread class
     */
    class WorkerThread
    {
    public:
        /**
         * Constructor
         */
        WorkerThread(int thread_id,
                     std::function<void(WorkUnitResult)> submit_result_callback);

        /**
         * Destructor
         */
        ~WorkerThread();

        /**
         * Tells the thread to start computing a work unit
         *
         * @param work_unit work unit to compute
         * @throws worker_thread::WorkerBusy if thread is already computing
         */
        void submit_work_unit(worker_pool::WorkUnit work_unit);

        /**
         * Returns true if the thread is currently computing a WorkUnit
         *
         * @returns true if thread is busy
         */
        bool is_busy();

    private:
        // Status vars
        std::atomic<bool> busy{false}; // True if thread is computing
        std::atomic<bool> run{true};   // Variable to notify thread of when to exit

        int thread_id;

        // Result callback
        std::function<void(worker_pool::WorkUnitResult)> result_callback;

        // Thread
        std::thread thread; // Actual controlled thread
        std::mutex thread_mutex;
        std::condition_variable thread_cv;

        // New work unit to be processed
        std::optional<worker_pool::WorkUnit> next_work_unit;

        // Pi generator
        pi::PiGenerator pi_gen;

        std::osyncstream osyncstream{std::cout};

        void start();
        void worker_function();
        void compute_new_work_unit();
        void submit_result(worker_pool::WorkUnitResult result);
    };

}
