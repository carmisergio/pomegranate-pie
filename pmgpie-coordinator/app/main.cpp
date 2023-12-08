/**
 * PMGPIe Coordinator entry point
 *
 * Main entry point for the program
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 07/12/2023
 */

#include <iostream>

#include "config.hpp"

// Main class
#include "pmgpie_coordinator.hpp"

int main(int argc, char *argv[])
{
    // Parse CLI arguments
    config::pmgpie_coordinator_config config;

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

    PMGPIeCoordinator pmgpie_coordinator(config);

    // Run main class
    pmgpie_coordinator.run();
}