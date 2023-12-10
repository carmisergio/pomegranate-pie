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
    struct pmgpie_coordinator_config
    {
        std::optional<unsigned short> port;
        std::optional<std::string> out_dir;
        std::optional<bool> overwrite;
    };
}
