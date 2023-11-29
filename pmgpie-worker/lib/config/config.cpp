/**
 * Configuration parser implementation
 *
 * Handles argument parsing and default configuration
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#include <iostream>
#include <vector>
#include <string_view>

#include "config.hpp"
#include "arg_strings.hpp"

/**
 * Parse CLI arguments
 *
 * @param argc CLI argument count
 * @param argv[] CLI arguments
 * @returns pmgpie_worker_config struct containing configuration with items as null
 *          if configuration was not provided
 * @throws config::ParseArgsError if any argument is invalid
 */
config::pmgpie_worker_config parse_args(int argc, char *argv[])
{
    // Limit arguments
    if (argc > 64)
        throw config::ParseArgsError();

    // Load arguments into vector
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    config::pmgpie_worker_config conf;

    // Iterate over arguments
    auto arg = args.begin();
    while (arg != args.end())
    {
        // -n or --node-id arg
        if (*arg == ARG_NODE_ID || *arg == ARG_NODE_ID_SHORT)
        {
            // Try to consume option
            // Check if there is an argument to consume
            arg++;
            if (arg != args.end())
                conf.worker_id = *arg;
            else
                throw config::ParseArgsError();
        }
        else
        {
            // Consume coordinator IP
            conf.coordinator_ip = *arg;
        }

        // Next arg
        arg++;
    }

    return conf;
}

/**
 * Prints CLI usage message
 *
 * @param argc CLI argument count
 * @param argv[] CLI arguments
 * @throws config::ParseArgsError if argument parsing fails
 */
void print_usage_message()
{
    std::cout << USAGE_MESSAGE << std::endl;
}

config::pmgpie_worker_config config::configure(int argc, char *argv[])
{

    config::pmgpie_worker_config conf;

    try
    {
        // Parse args
        conf = parse_args(argc, argv);

        // Check that required parameter is met
        if (!conf.coordinator_ip.has_value())
            throw config::ParseArgsError();
    }
    catch (config::ParseArgsError _)
    {
        // Print usage message
        print_usage_message();

        throw config::ParseArgsError();
    }

    return conf;
}

const char *config::ParseArgsError::what()
{
    return "Error parsing arguments";
}