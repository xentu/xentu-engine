#ifndef XENTU
#define XENTU


#define MAX_ARGV 12
//#define XEN_LOG_ENABLED 1


#ifndef XEN_LOG_ENABLED
#   define XEN_LOG(...)
#else
#   define XEN_LOG(...) printf(__VA_ARGS__)
#endif


int XEN_WARN(const char *format, ...);

int XEN_ERROR(const char *format, ...);


#endif