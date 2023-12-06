/**
 * Worker Thread Pool class implementation
 *
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 05/12/2023
 */

#include <cinttypes>

#include "worker_thread_pool.hpp"

worker_pool::WorkerThreadPool::WorkerThreadPool(unsigned short n_threads,
                                                std::function<void(worker_pool::WorkUnitResult)> result_callback)
{
    // Set result callback
    this->result_callback = result_callback;

    this->n_threads = n_threads;

    // Construcxt threads
    for (int i = 0; i < n_threads; i++)
    {

        this->worker_threads.push_back(std::make_shared<worker_pool::WorkerThread>(i,
                                                                                   [this](worker_pool::WorkUnitResult result)
                                                                                   { this->work_unit_result(result); }));
    }

    std::cout << "[THREAD POOL] " << n_threads << " threads ready!" << std::endl;
}

void worker_pool::WorkerThreadPool::submit_work_unit(worker_pool::WorkUnit work_unit)
{
    // Find non busy worker thread
    for (auto &thread : this->worker_threads)
    {
        if (!thread->is_busy())
        {
            thread->submit_work_unit(work_unit);
            return;
        }
    }

    throw worker_pool::AllWorkersBusy();
}

void worker_pool::WorkerThreadPool::work_unit_result(worker_pool::WorkUnitResult result)
{
    this->result_callback(result);
}
