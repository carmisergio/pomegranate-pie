/**
 * Argument constants and strings
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

#pragma once

// Arguments
#define ARG_PORT "--port"
#define ARG_PORT_SHORT "-p"
#define ARG_OUT_DIR "--out-dir"
#define ARG_OUT_DIR_SHORT "-d"
#define ARG_OVERWRITE "--overwrite"
#define ARG_OVERWRITE_SHORT "-o"
#define ARG_HELP "--help"
#define ARG_HELP_SHORT "-h"

// Messages
#define MSG_HELP "Usage: pmgpie-coordinator [OPTIONS]\nOptions: \n \t-p --port: Specify port the cluster server will be listening on\n \t-d --directory: Specify output directory\n \t-o --overwrite: Overwrite any results present in the output directory\n \t-h --help: Display this message"
#define MSG_OPT_VALUE_MISSING "pmgpie-coordinator: missing option value"
#define MSG_OPT_VALUE_INVALID "pmgpie-coordinator: invalid option value"
#define MSG_BAD_OPTION "pmgpie-coordinator: bad option"
#define MSG_TRY_HELP "Try 'pmgpie-coordinator --help' for more information"

// Defaults
#define DEFAULT_PORT 3141
#define DEFAULT_OUT_DIR "out"
#define DEFAULT_OVERWRITE false