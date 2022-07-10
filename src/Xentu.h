#ifndef XENTU
#define XENTU


#define MAX_ARGV 12


#if XEN_LOG_ENABLED
#   define XEN_LOG(...) printf(__VA_ARGS__)
#else
#   define XEN_LOG(...)
#endif


#endif