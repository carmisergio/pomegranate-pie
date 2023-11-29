/**
 * PMGPIeWorker Class
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#pragma once

#include "pmgpie_worker_config.hpp"

/**
 * PMGPIeWorker Class
 *
 * Main class
 */
class PMGPIeWorker
{

public:
    PMGPIeWorker(config::pmgpie_worker_config conf);
    void run();

private:
    config::pmgpie_worker_config conf; // Configuration
};
