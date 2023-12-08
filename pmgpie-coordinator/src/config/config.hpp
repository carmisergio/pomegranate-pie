/**
 * Configuration parser
 *
 * Handles argument parsing and default configuration
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#pragma once

#include <string>

#include "pmgpie_coordinator_config.hpp"

namespace config
{
    /**
     * Handle configuration
     *
     * @param argc CLI argument count
     * @param argv[] CLI arguments
     * @returns pmgpie_worker_config struct containing configuration
     */
    pmgpie_coordinator_config configure(int argc, char *argv[]);

    /**
     * Argument parsing error
     */
    class ParseArgsError : public std::exception
    {
    public:
        const char *what();
    };

    /**
     * Exception which signals to not continue execution but exit cleanly
     */
    class ExitCleanly : public std::exception
    {
    public:
        const char *what();
    };

}