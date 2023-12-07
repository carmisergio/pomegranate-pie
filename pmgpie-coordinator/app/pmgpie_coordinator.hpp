/**
 * PMGPIe Coordinator class
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 07/12/2023
 */

#pragma once

#include <atomic>

#include "ctrl_c.hpp"

/**
 * PMGPIeCoordinator Class
 *
 * Main class
 */
class PMGPIeCoordinator
{

public:
    PMGPIeCoordinator();
    void run();

private:
    // CTRL-C Handler
    void setup_ctrlc_handler();
    void quit();

    std::atomic<bool> running;
};
