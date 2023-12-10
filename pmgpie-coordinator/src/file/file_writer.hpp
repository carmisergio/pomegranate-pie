/**
 * FileWriter class
 *
 * @author Sergio Carmine 3CITI <me@sergiocarmi.net>
 * @date 09/12/2023
 */

#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <optional>

#define FILE_NAME_HEX "pi_hex.txt"

namespace file
{

    class FileWriter
    {
    public:
        FileWriter(std::filesystem::path output_dir, bool overwrite)
        {
            // Check if output directory exists
            if (!std::filesystem::is_directory(output_dir))
            {
                // Create output directory
                std::filesystem::create_directory(output_dir);
            }

            std::filesystem::path hex_file_path = output_dir;

            hex_file_path.append(FILE_NAME_HEX);

            // Open output files
            if (overwrite)
                hex_file.open(hex_file_path);
            else
                hex_file.open(hex_file_path, std::ios::app);
        }

        void write_hex(std::string digits)
        {
            this->hex_file << digits;
        }

        ~FileWriter()
        {
            if (hex_file.is_open())
                hex_file.close();
        }

    private:
        std::ofstream hex_file;
    };

    // Read in hex digits from file
    std::optional<std::string> read_hex_digits(std::filesystem::path out_dir);
}