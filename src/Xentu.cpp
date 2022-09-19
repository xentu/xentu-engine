#include <stdio.h>
#include <stdarg.h>

int XEN_WARN(const char *format, ...)
{
	#ifdef XEN_LOG_ENABLED
	// only print to the console if XEN_LOG is enabled.
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	#endif
	
	// todo: write to error log file or something.
	return -2;
}


int XEN_ERROR(const char *format, ...)
{
	// errors always output to the console.
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	
	// todo: write to error log file or something.
	return -3;
}