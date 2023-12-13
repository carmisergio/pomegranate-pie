/**
 * WorkUnitManager class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#pragma once

#include <atomic>
#include <string>
#include <list>
#include <optional>
#include <chrono>

#include "work_unit_combiner.hpp"

#define MAX_WORK_UNIT_SIZE 5000
#define WORK_UNIT_ORPHANED_TIMEOUT 10 // Seconds after which an orphaned
                                      // work unit is considered eligeble for dispatch

namespace work_unit_manager
{
    // Representation of a work unit
    struct WorkUnit
    {
        long long start;
        long long n_digits;
    };

    // Possible states of a work unit
    enum class WorkUnitState
    {
        OWNED,
        ORPHANED,
        DISOWNED,
    };

    // Info about a currently active work unit
    struct WorkUnitControlBlock
    {
        WorkUnit work_unit;                                  // Actual work unit
        WorkUnitState state;                                 // Current state of the work unit
        std::string worker_id;                               // Worker ID of the worker who owns or has
                                                             // orphaned this work unit
        std::chrono::system_clock::time_point orphaned_time; // Time at which this unit was orphaned
    };

    class WorkUnitManager
    {
    public:
        /**
         * Constructor
         *
         * @param start digit on which to start
         */
        WorkUnitManager(long long start, std::shared_ptr<work_unit_combiner::WorkUnitCombiner> work_unit_combiner)
            : next_digit{start}, work_unit_combiner(work_unit_combiner)
        {
        }

        /**
         * Get a work unit to compute
         *
         * @param worker_id worker id of the worker requesting the work unit
         */
        WorkUnit assign_work_unit(std::string worker_id)
        {
            // First, try to find a work unit that has been disowned
            std::optional<WorkUnit> disowned = find_available_work_unit(worker_id);

            // If there was one, return it
            if (disowned.has_value())
                return disowned.value();

            // Create a new work unit
            return dispatch_new_work_unit(worker_id);
        }

        /**
         * Mark all work units with a worker_id as owned
         *
         * @param worker_id
         */
        void mark_owned(std::string worker_id)
        {
            mark_all(worker_id, WorkUnitState::OWNED);
        }

        /**
         * Mark all work units with a worker_id as orphaned
         *
         * @param worker_id
         */
        void mark_orphaned(std::string worker_id)
        {
            mark_all(worker_id, WorkUnitState::ORPHANED);
        }

        /**
         * Mark all work units with a worker_id as disowned
         *
         * @param worker_id
         */
        void mark_disowned(std::string worker_id)
        {
            mark_all(worker_id, WorkUnitState::DISOWNED);
        }

        /**
         * Submit result of a work unit
         *
         * @param digits compute digits
         * @param start work unit start
         */
        void submit_result(std::string digits, long long start)
        {
            // std::cout << "[WORK UNIT MANAGER] Removing work unit: (start = " << start << ")" << std::endl;

            // Submit result to combiner
            this->work_unit_combiner->submit_work_unit_result(digits, start);

            // Remove work unti from active list
            remove_work_unit(start);
        }

    private:
        // Mark all work units with specific worker ID with a state
        void mark_all(std::string &worker_id, WorkUnitState state)
        {
            // Lock active work units list
            std::unique_lock(this->active_work_units_mutex);

            // Do mark
            for (auto &wucb : this->active_work_units)
            {
                if (wucb.worker_id == worker_id)
                {
                    // Set state
                    wucb.state = state;

                    // Set orphaned time if state is orphaned
                    if (state == WorkUnitState::ORPHANED)
                        wucb.orphaned_time = std::chrono::high_resolution_clock::now();

                    // Unset worker id  if state is disowned
                    if (state == WorkUnitState::DISOWNED)
                        wucb.worker_id = "";
                }
            }
        }

        // Find available work unit and if found, mark it as owned by worker_id
        std::optional<WorkUnit> find_available_work_unit(std::string worker_id)
        {

            // Lock active work units list
            std::unique_lock(this->active_work_units_mutex);

            std::optional<WorkUnit> res;

            // Search
            for (auto &wucb : this->active_work_units)
            {
                if (wucb.state == WorkUnitState::DISOWNED ||
                    wucb.state == WorkUnitState::ORPHANED && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - wucb.orphaned_time).count() > WORK_UNIT_ORPHANED_TIMEOUT)
                {
                    // Get work unit info
                    res = wucb.work_unit;

                    // Change work unit state
                    wucb.state = WorkUnitState::OWNED;
                    wucb.worker_id = worker_id;

                    break;
                }
            }

            return res;
        }

        // Generate a new unit, add it to the active list and return it
        WorkUnit dispatch_new_work_unit(std::string worker_id)
        {
            // Generate new work unit
            WorkUnit work_unit = gen_work_unit();

            // Generate new WorkUnitControlBlock
            WorkUnitControlBlock work_unit_cb{
                work_unit,
                WorkUnitState::OWNED,
                worker_id};

            // Lock active work units list
            std::unique_lock(this->active_work_units_mutex);

            // Add it to the active list
            this->active_work_units.push_back(work_unit_cb);

            return work_unit;
        }

        //  Generate new work unit
        WorkUnit gen_work_unit()
        {
            WorkUnit res;

            // Calculate number of digits to give
            res.n_digits = MAX_WORK_UNIT_SIZE /
                           (next_digit == 0 ? 1 : next_digit * log10((double)next_digit) / (MAX_WORK_UNIT_SIZE * 1.5));

            if (res.n_digits < 1)
                res.n_digits = 1;

            res.start = next_digit;

            // Update next start
            next_digit += res.n_digits;

            return res;
        }

        // Remove work unit with specific start
        void remove_work_unit(long long start)
        {
            // Lock active work units list
            std::unique_lock(this->active_work_units_mutex);

            // Remove unit with this start
            this->active_work_units.remove_if([start](WorkUnitControlBlock wucb)
                                              { return wucb.work_unit.start == start; });
        }

        std::atomic<long long> next_digit; // Next digit from which to dispatch work units

        // Main list of all work units we're working on
        std::list<WorkUnitControlBlock> active_work_units;
        std::mutex active_work_units_mutex;

        // Pointer to the work unit combiner object
        std::shared_ptr<work_unit_combiner::WorkUnitCombiner> work_unit_combiner;
    };
}