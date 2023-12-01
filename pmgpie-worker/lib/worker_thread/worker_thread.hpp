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

#include "pi.hpp"
#include "worker_thread.hpp"

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
     * Representation of a work unit result
     */
    class WorkUnitResult
    {
    public:
        // Pseudo-constructors
        static WorkUnitResult succesful(std::string digits, long long start);
        static WorkUnitResult unsuccesful();

        // Getters
        bool success();
        std::string get_digits();
        long long get_start();

    private:
        bool was_succesful; // Whether the computation was succesful or not
        std::string digits;
        long long start;
    };

    /**
     * WorkerThread class
     */
    class WorkerThread
    {
    public:
        /**
         * Constructor
         */
        WorkerThread(int thread_id);

        /**
         * Start worker thread
         */
        void start();

        /**
         * Stop worker thread
         */
        void stop();

        /**
         * Set result callback
         *
         * @param callback callback funciton
         */
        void set_result_callback(std::function<void(WorkUnitResult)> callback);

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

        int thread_id;

        // Result callback
        std::optional<std::function<void(WorkUnitResult)>> result_callback;

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
        void submit_result(WorkUnitResult result);
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
