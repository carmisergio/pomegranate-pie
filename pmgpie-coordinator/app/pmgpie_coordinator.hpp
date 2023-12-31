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
#include "work_unit_combiner.hpp"
#include "file_writer.hpp"
#include "frontend.hpp"
#include "stats.hpp"

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

    std::shared_ptr<PMGPIeClusterStats> stats;

    std::shared_ptr<file::FileWriter> file_writer;
    std::shared_ptr<work_unit_combiner::WorkUnitCombiner> work_unit_combiner;
    std::shared_ptr<work_unit_manager::WorkUnitManager> work_unit_manager;
    std::shared_ptr<pmgpie_cluster_server::PMGPIeClusterServer> pmgpie_cluster_server;
    std::shared_ptr<frontend::FrontendServer> frontend_server;
};
