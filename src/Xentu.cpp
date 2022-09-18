#ifndef XENTU_CPP
#define XENTU_CPP

#include <stdio.h>
#include <stdarg.h>

void XEN_WARN(char *format, ...)
{
	#ifdef XEN_LOG_ENABLED
	// only print to the console if XEN_LOG is enabled.
	va_list args;
	vprintf(format, args);
	#endif
	
	// todo: write to error log file or something.
}


void XEN_ERROR(char *format, ...)
{
	// errors always output to the console.
	va_list args;
	vprintf(format, args);
	
	// todo: write to error log file or something.
}


#endif