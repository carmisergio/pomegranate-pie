/**
 * PMGPIeCoordinator Implementation
 *
 * Main class
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 07/12/2023
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <memory>

#include "pmgpie_coordinator.hpp"

/**
 * Constructor
 */
PMGPIeCoordinator::PMGPIeCoordinator(config::pmgpie_coordinator_config conf)
    : conf(conf)
{
    std::string initial_digits;

    // Load previously computed digits
    if (!conf.overwrite.value())
    {
        auto read_result = file::read_hex_digits(conf.out_dir.value());
        if (read_result.has_value())
        {
            std::cout << "[SETUP] Loaded previously computed digits: " << read_result.value().size() << std::endl;

            initial_digits = read_result.value();
        }
    }

    this->stats = std::make_shared<PMGPIeClusterStats>();

    // Construct all objects
    this->file_writer = std::make_shared<file::FileWriter>(std::filesystem::path(conf.out_dir.value()), conf.overwrite.value());
    this->work_unit_combiner = std::make_shared<work_unit_combiner::WorkUnitCombiner>(file_writer, initial_digits, stats);
    this->work_unit_manager = std::make_shared<work_unit_manager::WorkUnitManager>(initial_digits.size(), this->work_unit_combiner);
    this->pmgpie_cluster_server = std::make_shared<pmgpie_cluster_server::PMGPIeClusterServer>(conf.port.value(), work_unit_manager, stats);
    this->frontend_server = std::make_shared<frontend::FrontendServer>(stats);
}

/**
 * Main entry point
 */
void PMGPIeCoordinator::run()
{

    // Run
    this->running = true;

    // Setup CTRL-C events
    this->setup_ctrlc_handler();

    while (this->running.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // this->tcp_server.send_all("Goodbye!");

    std::cout << "Exiting..." << std::endl;
}

/**
 * Sets up the CTRL C handler
 */
void PMGPIeCoordinator::setup_ctrlc_handler()
{
    // Register CTRL-C handler
    unsigned int handler_id = CtrlCLibrary::SetCtrlCHandler(
        [this](enum CtrlCLibrary::CtrlSignal event) -> bool
        {
            if (event == CtrlCLibrary::kCtrlCSignal)
                this->quit();

            return true;
        });
}

/**
 * Handle CTRL-C events
 */
void PMGPIeCoordinator::quit()
{
    // Stop main thread
    this->running = false;
}