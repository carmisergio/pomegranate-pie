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

#include "pmgpie_coordinator_config.hpp"
#include "ctrl_c.hpp"
#include "pmgpie_cluster_server.hpp"
#include "work_unit_manager.hpp"

/**
 * PMGPIeCoordinator Class
 *
 * Main class
 */
class PMGPIeCoordinator
{

public:
    PMGPIeCoordinator(config::pmgpie_coordinator_config conf);
    void run();

private:
    // CTRL-C Handler
    void setup_ctrlc_handler();
    void quit();

    config::pmgpie_coordinator_config conf;

    std::atomic<bool> running;

    std::shared_ptr<work_unit_manager::WorkUnitManager> work_unit_manager;
    pmgpie_cluster_server::PMGPIeClusterServer pmgpie_cluster_server;
};
