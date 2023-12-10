/**
 * FileWriter class implementation
 *
 * @author Sergio Carmine 3CITI <me@sergiocarmi.net>
 * @date 09/12/2023
 */

#include <file_writer.hpp>

std::optional<std::string> file::read_hex_digits(std::filesystem::path out_dir)
{
    std::optional<std::string> result;

    if (!std::filesystem::is_directory(out_dir))
        return result;

    // Cosntruct path
    std::filesystem::path hex_file_path = out_dir;
    hex_file_path.append(FILE_NAME_HEX);

    if (!std::filesystem::is_regular_file(hex_file_path))
        return result;

    // Open file
    std::ifstream hex_file(hex_file_path);

    // Read file
    std::string read((std::istreambuf_iterator<char>(hex_file)),
                     std::istreambuf_iterator<char>());

    result = read;

    return result;
}