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

int main()
{
    std::cout << "Test" << std::endl;

    PMGPIeWorker pmgpie_worker;

    // Run main class
    pmgpie_worker.run();
}