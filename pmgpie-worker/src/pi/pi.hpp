/**
 * Pi generation
 *
 * Handles generating arbitrary hexadecimal digits of pi
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 30/11/2023
 */

#pragma once

#include <string>
#include <atomic>

namespace pi
{

    class PiGenerator
    {
    public:
        /**
         * Generate arbitrary amount of HEX pi digits starting from specific index
         *
         * @param start starting digit eg. 0 for 3
         * @param n number of digits to generate
         *
         * @returns string containing hex digits of pi
         */
        std::string generate_pi_digits(long long start, int n);

        /**
         * Interrupt computation if one is under way
         */
        void interrupt();

    private:
        std::atomic<bool> run = true; // Way of interrupting long calculations

        long long mod_pow(long long base, long long exp, long long mod);
        double bbp_factor(int j, long long n);
        short compute_pi_digit_bbp(long long start);
        void check_stop();
    };

    /**
     * Pi computation error
     */
    class PiComputeError : public std::exception
    {
    public:
        const char *what();
    };

    /**
     * Execution interrupted
     */
    class ExecutionInterrupted : public std::exception
    {
    public:
        const char *what();
    };

}