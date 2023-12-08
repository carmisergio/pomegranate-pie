/**
 * Configuration parser implementation
 *
 * Handles argument parsing and default configuration
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#include <iostream>
#include <vector>
#include <string_view>

#include "config.hpp"
#include "arg_strings.hpp"

/**
 * Prints CLI message
 *
 * @param msg message to print
 */
void print_message(std::string msg)
{
    std::cout << msg << std::endl;
}

/**
 * Parse CLI arguments
 *
 * @param argc CLI argument count
 * @param argv[] CLI arguments
 * @returns pmgpie_coordinator_config struct containing configuration with items as null
 *          if configuration was not provided
 * @throws config::ParseArgsError if any argument is invalid
 */
config::pmgpie_coordinator_config parse_args(int argc, char *argv[])
{
    // Limit arguments
    if (argc > 64)
        throw config::ParseArgsError();

    // Load arguments into vector
    const std::vector<std::string> args(argv + 1, argv + argc);

    config::pmgpie_coordinator_config conf;

    bool display_help = false;

    // Iterate over arguments
    auto arg = args.begin();
    while (arg != args.end())
    {
        if (*arg == ARG_PORT || *arg == ARG_PORT_SHORT)
        {
            // Try to consume option
            // Check if there is an argument to consume
            arg++;
            if (arg != args.end())
            {
                try
                {
                    conf.port = std::stoi(*arg);
                }
                catch (std::exception _)
                {
                    print_message(MSG_OPT_VALUE_INVALID);
                    throw config::ParseArgsError();
                }
            }
            else
            {
                print_message(MSG_OPT_VALUE_MISSING);
                throw config::ParseArgsError();
            }
        }
        // -h or --heallp arg
        else if (*arg == ARG_HELP || *arg == ARG_HELP_SHORT)
        {
            print_message(MSG_HELP);
            throw config::ExitCleanly();
        }
        else
        {
            print_message(MSG_BAD_OPTION);
            throw config::ParseArgsError();
        }

        // Next arg
        arg++;
    }

    return conf;
}

/**
 * Sets default on a pmgpie_coordinator_config object
 *
 * @param config config object to modify
 */
void set_defaults(config::pmgpie_coordinator_config &config)
{
    if (!config.port.has_value())
        config.port = DEFAULT_PORT;
}

config::pmgpie_coordinator_config config::configure(int argc, char *argv[])
{

    config::pmgpie_coordinator_config conf;

    try
    {
        // Parse args
        conf = parse_args(argc, argv);
    }
    catch (std::exception &e)
    {
        // Print "try --help" message
        print_message(MSG_TRY_HELP);

        // Print usage message
        throw;
    }

    // Set default values
    set_defaults(conf);

    return conf;
}

const char *config::ParseArgsError::what()
{
    return "Error parsing arguments";
}

const char *config::ExitCleanly::what()
{
    return "Cannot continue execution";
}