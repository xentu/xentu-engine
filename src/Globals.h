#pragma once

#include <string>

// max executable input args.
#define MAX_ARGV 12

// texture constants
#define TX_REPEAT 0
#define TX_CLAMP_TO_EDGE 1
#define TX_CLAMP_TO_BORDER 2


#define XEN_ECHO(...) printf(__VA_ARGS__)


/**
 * Log a warning even if XEN_DEBUG is not defined.
 */
int XEN_DEBUG(const char *format, ...);

/**
 * Log an error even if XEN_DEBUG is not defined.
 */
int XEN_ERROR(const char *format, ...);


bool XEN_FILE_EXISTS(const std::string& name);