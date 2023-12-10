/**
 * WorkUnitcombiner class implementation
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 09/12/2023
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <syncstream>
#include <stats.hpp>

#include "tsqueue.hpp"
#include "file_writer.hpp"

namespace work_unit_combiner
{
    struct InternalWorkUnitResult
    {
        std::string digits;
        long long start;
        bool processor_exit;
    };

    class WorkUnitCombiner
    {
    public:
        WorkUnitCombiner(
            std::shared_ptr<file::FileWriter> file_writer,
            std::string initial_digits,
            std::shared_ptr<PMGPIeClusterStats> stats)
            : file_writer(file_writer),
              processor_thr(&WorkUnitCombiner::processor, this),
              next_digit{0},
              stats(stats)

        {
            // Prime with initial digits
            if (initial_digits.size() > 0)
            {
                this->next_digit = initial_digits.size();
                new_sequential_digits(initial_digits, false);
            }
        }

        /**
         * Process a new work unit result
         */
        void submit_work_unit_result(std::string &digits, long long start)
        {
            this->to_process.push(InternalWorkUnitResult{digits, start, false});
        }

        ~WorkUnitCombiner()
        {
            // Send special internal work unit to make the processor thread exit
            this->to_process.push(InternalWorkUnitResult{"", 0, true});

            if (this->processor_thr.joinable())
                this->processor_thr.join();
        }

    private:
        void processor()
        {
            while (true)
            {
                // Get new work unit result to process
                InternalWorkUnitResult new_result = this->to_process.pop();

                // Handle exiting the processor
                if (new_result.processor_exit)
                    break;

                // Add it to the processing area
                this->processing_area.push_back(new_result);

                // Process
                process();
            }
        }

        void process()
        {
            std::string new_digits = "";

            while (true)
            {
                // std::cout << "Processing..." << std::endl;
                // Try to find work unit that's needed
                std::vector<InternalWorkUnitResult>::iterator next = find_next_work_unit();

                // If it isn't present, we can't process further
                if (next == this->processing_area.end())
                    break;

                // std::cout << "Next result found" << std::endl;

                InternalWorkUnitResult tmp = *next;

                // Remove from processing area
                this->processing_area.erase(next);

                new_digits.append(tmp.digits);

                // Update next
                this->next_digit += tmp.digits.size();
            }

            // Check if any new sequential digits have been added
            if (new_digits.size() > 0)
                new_sequential_digits(new_digits);
        }

        // This function gets called with all sequential digits that are inputed
        void new_sequential_digits(std::string &new_digits, bool write = true)
        {

            if (write)
            {
                file_writer->write_hex(new_digits);
            }

            // std::cout
            //     << "[PI] Added " << new_digits.size() << " digits" << std::endl;
            std::osyncstream(std::cout) << "[PI] HEX digits: " << this->next_digit << std::endl;
            stats->hex_digits_generated = this->next_digit;
        }

        // Search the processing area for the next work unit
        std::vector<InternalWorkUnitResult>::iterator find_next_work_unit()
        {
            for (auto i = this->processing_area.begin(); i < this->processing_area.end(); i++)
            {
                if (i->start == this->next_digit)
                    return i;
            }

            return this->processing_area.end();
        }

        long long next_digit; // First missing digit of pi

        std::thread processor_thr;

        std::vector<InternalWorkUnitResult> processing_area;

        TSQueue<InternalWorkUnitResult> to_process;

        std::shared_ptr<file::FileWriter> file_writer;

        std::shared_ptr<PMGPIeClusterStats> stats;
    };
}