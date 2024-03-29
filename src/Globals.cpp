#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>

int XEN_WARN(const char *format, ...)
{
	#ifdef XEN_DEBUG
	// only print to the console if XEN_ECHO is enabled.
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


bool XEN_FILE_EXISTS(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}