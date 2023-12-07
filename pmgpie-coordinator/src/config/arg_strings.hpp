/**
 * Argument constants and strings
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 29/11/2023
 */

#pragma once

#include <string>

// Arguments
#define ARG_NODE_ID "--node-id"
#define ARG_NODE_ID_SHORT "-n"
#define ARG_THREADS "--threads"
#define ARG_THREADS_SHORT "-t"
#define ARG_HELP "--help"
#define ARG_HELP_SHORT "-h"

// Messages
#define MSG_HELP "Usage: pmgpie-worker [OPTIONS] COORDINATOR_HOST\nOptions: \n \t-n --node-id: Specify worker node id\n \t-t --threads: Number of worker threads\n \t-h --help: Display this message"
#define MSG_COORD_HOST_MISSING "pmgpie-worker: missing coordinator host"
#define MSG_OPT_VALUE_MISSING "pmgpie-worker: missing option value"
#define MSG_OPT_VALUE_INVALID "pmgpie-worker: invalid option value"
#define MSG_BAD_OPTION "pmgpie-worker: bad option"
#define MSG_TRY_HELP "Try 'pmgpie-worker --help' for more information"