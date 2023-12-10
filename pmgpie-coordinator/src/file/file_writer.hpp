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

#define FILE_NAME_DEC "pi_dec.txt"
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

            std::filesystem::path dec_file_path = output_dir;
            std::filesystem::path hex_file_path = output_dir;

            dec_file_path.append(FILE_NAME_DEC);
            hex_file_path.append(FILE_NAME_HEX);

            // Open output files
            if (overwrite)
            {
                dec_file.open(dec_file_path);
                hex_file.open(hex_file_path);
            }
            else
            {
                dec_file.open(dec_file_path, std::ios::app);
                hex_file.open(hex_file_path, std::ios::app);
            }
        }

        void write_dec(std::string digits)
        {
            this->dec_file << digits;
        }

        void write_hex(std::string digits)
        {
            this->hex_file << digits;
        }

        ~FileWriter()
        {
            if (dec_file.is_open())
                dec_file.close();
            if (hex_file.is_open())
                hex_file.close();
        }

    private:
        std::ofstream dec_file;
        std::ofstream hex_file;
    };

    // Read in hex digits from file
    std::optional<std::string> read_hex_digits(std::filesystem::path out_dir);
}