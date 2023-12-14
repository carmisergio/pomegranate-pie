/**
 * HEX2DEC
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 14/12/2023
 */

#include <iostream>
#include <fstream>
#include <filesystem>

// Libraries
#include "config.hpp" // CLI parser
#include "hex_aggregator.hpp"

std::string load_file_string(std::filesystem::path in_file)
{
    // Open file
    std::ifstream file(in_file);

    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file");
    }

    // Read file
    std::string read((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    file.close();

    return read;
}

void write_file_string(std::filesystem::path out_file, std::string str)
{
    std::ofstream file(out_file);

    if (!file.is_open())
    {
        throw std::runtime_error("Error writing file");
    }

    // Write file
    file << str;

    file.close();
}

int main(int argc, char *argv[])
{
    // Parse CLI arguments
    config::hex2dec_config config;
    try
    {
        config = config::configure(argc, argv);
    }
    catch (config::ParseArgsError _)
    {
        return 1;
    }
    catch (config::ExitCleanly _)
    {
        return 0;
    }

    std::cout << "Converting \"" << config.input_file.value() << "\"..." << std::endl;

    // Construce path
    std::filesystem::path in_file(config.input_file.value());

    // Load file string
    std::string hex_string;
    try
    {
        hex_string = load_file_string(in_file);
    }
    catch (std::exception &e)
    {
        std::cout << "Error opening file: " << in_file << std::endl;
        return 2;
    }

    HexAggregator hexagg;

    try
    {

        hexagg.insert_digits(hex_string);
    }
    catch (std::exception &e)
    {
        std::cout << "HEX Conversion error!: " << e.what() << std::endl;
        return 3;
    }

    std::filesystem::path out_file(config.output_file.value());

    std::cout << "Saving output to: \"" << config.output_file.value() << "\"" << std::endl;

    // Write file string
    try
    {
        write_file_string(out_file, hexagg.get_dec());
    }
    catch (std::exception &e)
    {
        std::cout << "Error writing to file: " << out_file << std::endl;
        return 4;
    }

    std::cout << "Done!" << std::endl;
}