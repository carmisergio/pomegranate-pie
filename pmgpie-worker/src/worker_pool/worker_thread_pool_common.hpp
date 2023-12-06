/**
 * Worker Thread Pool common types
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 05/12/2023
 */

#pragma once

#include <string>

namespace worker_pool
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
        WorkUnitResult(std::string digits, long long start)
        {
            this->digits = digits;
            this->start = start;
        }

        std::string get_digits()
        {
            return this->digits;
        }

        long long get_start()
        {
            return this->start;
        }

        std::string digits;
        long long start;
    };

    /**
     * Worker is already busy exception
     */
    class WorkerBusy : public std::exception
    {
    public:
        const char *what()
        {
            return "Worker is already busy!";
        }
    };

    /**
     * All workers busy exception
     */
    class AllWorkersBusy : public std::exception
    {
    public:
        const char *what()
        {
            return "All Worker threads are already busy!";
        }
    };
}