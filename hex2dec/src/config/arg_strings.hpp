/**
 * Argument constants and strings
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 14/12/2023
 */

#pragma once

#include <string>

// Arguments
#define ARG_OUT "--out"
#define ARG_OUT_SHORT "-o"
#define ARG_HELP "--help"
#define ARG_HELP_SHORT "-h"

// Messages
#define MSG_HELP "Usage: hex2dec [OPTIONS] INPUT_FILE\nOptions: \n \t-o --out: Out file\n \t-h --help: Display this message"
#define MSG_INPUT_FILE_MISSING "hex2dec: missing input file"
#define MSG_OPT_VALUE_MISSING "hex2dec: missing option value"
#define MSG_OPT_VALUE_INVALID "hex2dec: invalid option value"
#define MSG_BAD_OPTION "hex2dec: bad option"
#define MSG_TRY_HELP "Try 'hex2dec--help' for more information"

// Defaults
#define DEFAULT_OUT "pi_dec.txt"