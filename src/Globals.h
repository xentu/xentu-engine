#pragma once

#include <string>

#define MAX_ARGV 12


#ifndef XEN_DEBUG
#   define XEN_ECHO(...)
#else
    // Output a message to the console if XEN_DEBUG is enabled.
#   define XEN_ECHO(...) printf(__VA_ARGS__)
#endif


/**
 * Log a warning even if XEN_DEBUG is not defined.
 */
int XEN_WARN(const char *format, ...);

/**
 * Log an error even if XEN_DEBUG is not defined.
 */
int XEN_ERROR(const char *format, ...);