/**
 * Configuration result struct
 *
 * Result of configuration to be used in the program
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#pragma once

#include <string>
#include <optional>

namespace config
{
    // Configuration result
    struct pmgpie_worker_config
    {
        std::optional<std::string> coordinator_ip; // Coordinator ip address
        std::optional<std::string> worker_id;      // Worker name override
    };
}
