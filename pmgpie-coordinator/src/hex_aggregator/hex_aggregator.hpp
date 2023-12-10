/**
 * HexAggregator
 *
 * Receives hex digits sequentially and is responsible for constructing the
 * final decimal output
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 26/11/2023
 */

#pragma once

#include <gmpxx.h>
#include <string>

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