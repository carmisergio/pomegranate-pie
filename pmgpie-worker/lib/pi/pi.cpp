/**
 * Pi generation implementation
 *
 * Handles generating an arbitrary number of digits of pi
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 30/11/2023
 */

#include <cmath>

#include "pi.hpp"

/**
 * Hexadecimal conversion failure
 */
class HexConversionError : public std::exception
{
public:
    const char *what();
};

/**
 * Get fractional part of negative number
 *
 * @param x input value
 * @returns fractional part of x
 */
double modf_neg(double x)
{
    double _;
    double res = std::modf(x, &_);

    return res < 0 ? res + 1 : res;
}

/**
 * Get char value of hex digit
 *
 * @param digit digit as number 0-15
 * @returns digit as char
 */
char hex_digit(short digit)
{
    char res;

    // Digits 0 - 9
    if (digit < 10)
        res = (char)('0' + digit);

    // Digits a - f
    else if (digit <= 15)
        res = (char)('a' + digit - 10);

    // Invalid digit
    else
        throw HexConversionError();

    return res;
}

/**
 * Perform modular exponentiation
 *
 * @param base b
 * @param exp e
 * @param mod m
 * @returns value of modular exponentiation
 */
long long pi::PiGenerator::mod_pow(long long base, long long exp, long long mod)
{
    long long res = 1;

    base = base % mod;

    // Step through bits of exponent
    while (exp > 0)
    {
        if (exp & 1)
            res = (res * base) % mod;

        base = (base * base) % mod;

        exp >>= 1;
    }

    return res;
}

/**
 * Computes factor S(j,n) of BBP formula
 *
 * @param j j
 * @param n n
 * @returns result
 */
double pi::PiGenerator::bbp_factor(int j, long long n)
{
    // Left summation
    double left = 0;
    for (long long k = 0; k <= n; k++)
    {
        // Check if we have to stop
        this->check_stop();

        long long r = 8 * k + j;
        double _;
        left = std::modf((double)left + (double)mod_pow(16, n - k, r) / (double)r, &_);
    }

    // Right summation
    double right = 0;
    for (long long k = n + 1;; k++)
    {
        // Check if we have to stop
        this->check_stop();

        double rnew = right + pow(16, n - k) / (8 * k + j);

        // Stop when there's no change
        if (right == rnew)
            break;

        right = rnew;
    }

    return left + right;
}

/**
 * Compute digit of pi at given index
 *
 * @param start index of digit
 * @returns digit
 * @throws HexConversionError if digit is invalid
 */
short pi::PiGenerator::compute_pi_digit_bbp(long long start)
{
    // Count starting 3
    start -= 1;

    // Compute factors
    double factors = 4.0 * bbp_factor(1, start) - 2.0 * bbp_factor(4, start) - bbp_factor(5, start) - bbp_factor(6, start);

    // Get only fractional part
    double modded = modf_neg(factors);

    // Shift left to get correct digit
    double res = 16 * modded;

    return floor(res);
}

void pi::PiGenerator::check_stop()
{
    if (!this->run.load())
        throw pi::ExecutionInterrupted();
}

std::string pi::PiGenerator::generate_pi_digits(long long start, int n)
{
    std::string digits = "";

    // Compute all digits
    for (long long i = start; i < start + n; i++)
    {
        // Check if we have to stop
        this->check_stop();

        // Compute digit
        short digit = compute_pi_digit_bbp(i);

        // Convert to hex char
        char digit_char;
        try
        {
            digit_char = hex_digit(digit);
        }
        catch (HexConversionError &e)
        {
            throw pi::PiComputeError();
        }

        // Append to result
        digits += digit_char;
    }

    return digits;
}

void pi::PiGenerator::interrupt()
{
    // Notify computation functions to stop
    this->run = false;
}

const char *pi::PiComputeError::what()
{
    return "Pi calculation error";
}

const char *pi::ExecutionInterrupted::what()
{
    return "Execution interrupted";
}