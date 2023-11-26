/**
 * HexAggregator
 *
 * Receives hex digits sequentially and is responsible for constructing the
 * final decimal output
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 26/11/2023
 */

#include <gmpxx.h> // Arbitrary precision library
#include <string>
#include <exception>
#include <iostream>
#include <iomanip>

/**
 * Converts hex char (0-9,a-f/A-F) to integer form (0-15)
 *
 * @param in input character
 * @returns integer value of input character
 *
 * @throws std::out_of_range if input character is not a valid hex digit
 */
short hex_char_to_int(char in)
{
    // Numbers 0-9
    if (in >= '0' && in <= '9')
        return in - '0';

    // Letters a-f
    if (in >= 'a' && in <= 'f')
        return in - 'a';

    // Letters A-F
    if (in >= 'A' && in <= 'F')
        return in - 'A';

    // Input character was invalid
    throw std::out_of_range("Invalid hex character: " + std::to_string(in));
}

/**
 * Get decimal string of mpf_class
 *
 * @param n input number
 * @param precision number of decimal digits to get
 * @returns decimal string
 */
std::string get_dec_string(mpf_class &n, long long precision)
{
    std::stringstream ss;
    ss << std::setprecision(precision) << n;

    return ss.str();
}

/**
 * HexAggregator class
 *
 * Handles converting sequential hex digits to decimal
 */
class HexAggregator
{

public:
    HexAggregator(); // Constructor
    void insert_digits(std::string digits);
    void insert_digit(short digit);
    long long n_digits_dec();
    std::string get_dec();

private:
    // Conversion variables
    mpf_class result;
    mpf_class power_of_16;
    mpf_class digit_dec;
    long long digit_count;
};

/**
 * HexAggregator constructor
 *
 */
HexAggregator::HexAggregator()
{
    // Set start values
    this->result = 0;
    this->power_of_16 = 1;
    this->digit_count = 0;
}

/**
 * Insert new N hexadecimal digits
 *
 * @param digits hexadecimal digits (0-f)
 */
void HexAggregator::insert_digits(std::string digits)
{
    // Loop over digits
    for (char digit : digits)
    {
        insert_digit(hex_char_to_int(digit));
    }
}

/**
 * Insert 1 new hexadecimal digit
 *
 * @param digit hexadecimal digit (0-15)
 *
 * @throws std::out_of_range if hex digit is invalid
 */
void HexAggregator::insert_digit(short digit)
{
    // Check if invalid digit
    if (digit < 0 || digit > 15)
        throw std::out_of_range("Invalid hexadecimal digit: " + std::to_string(digit));

    // Increment digit counter
    this->digit_count++;

    // Make sure enough space is reserved for the new digit
    // NOTE: 1 hex digit = 4 bin digits
    this->result.set_prec(digit_count * 4);
    this->power_of_16.set_prec(digit_count * 4);
    this->digit_dec.set_prec(digit_count * 4);

    // Add the digit
    this->digit_dec = digit;
    this->digit_dec = this->digit_dec / this->power_of_16; // Dec value of digit
    this->result += this->digit_dec;

    // Prepare for next digit
    this->power_of_16 *= 16;
}

/**
 * Get number of certain decimal digits
 *
 * @returns number of certain decimal digits
 */
long long HexAggregator::n_digits_dec()
{
    // Get string of last digit and current result
    std::string power_of_16 = get_dec_string(this->digit_dec, this->digit_count * 4);
    std::string cur_result_str = get_dec_string(this->result, this->digit_count * 4);

    std::cout << "After strings" << std::endl;
    std::cout << cur_result_str.length() << std::endl;
    std::cout << power_of_16 << std::endl;

    // Find last 0 on the right of the decimal part of the last digit
    // NOTE: n_digits = 2 to skip integer and decimal point
    unsigned long long n_digits = 2;
    while (n_digits < cur_result_str.length() && power_of_16[n_digits] != '0')
        n_digits++;

    n_digits--; // Move back last incorrect digit

    std::cout << "Mids" << std::endl;

    // Move back to account for potential future carries in the result
    // when digit is 9
    while (n_digits > 0 && cur_result_str[n_digits] == '9')
        n_digits--;

    // Account for potential of overflow when digit is actually multiplied
    n_digits--;

    return n_digits;
}

/**
 * Get string of certain decimal digits
 *
 * @returns string of certain decimal digits
 */
std::string HexAggregator::get_dec()
{
    // Find number of certain digits
    long long n_digits = this->n_digits_dec();

    // Get string
    return get_dec_string(this->result, n_digits);
}