/**
 * PMGPIe Worker entry point
 *
 * Main entry point for the program
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#include <iostream>

// Main class
#include "pmgpie_worker.hpp"

// Libraries
#include "config.hpp" // CLI parser

int main(int argc, char *argv[])
{
    // Parse CLI arguments
    config::pmgpie_worker_config config;
    try
    {
        config = config::configure(argc, argv);
    }
    catch (config::ParseArgsError _)
    {
        return 1;
    }
    catch (config::ExitCleanly _)
    {
        return 0;
    }

    PMGPIeWorker pmgpie_worker(config);

    // Run main class
    pmgpie_worker.run();
}