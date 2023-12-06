/**
 * Worker Thread Pool class
 *
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 05/12/2023
 */

#pragma once

#include <functional>

#include "worker_thread_pool_common.hpp"
#include "worker_thread.hpp"

namespace worker_pool
{

    class WorkerThreadPool
    {
    public:
        /**
         * Constructor WorkerThreadPool
         */
        WorkerThreadPool(unsigned short n_threads, std::function<void(worker_pool::WorkUnitResult)> result_callback);

        /**
         * Tells the thread to start computing a work unit
         *
         * @param work_unit work unit to compute
         * @throws worker_thread::WorkerBusy if thread is already computing
         */
        void submit_work_unit(worker_pool::WorkUnit work_unit);

        int n_threads;

    private:
        std::vector<std::shared_ptr<worker_pool::WorkerThread>> worker_threads;
        std::function<void(worker_pool::WorkUnitResult)> result_callback;

        void work_unit_result(worker_pool::WorkUnitResult result);
    };

}