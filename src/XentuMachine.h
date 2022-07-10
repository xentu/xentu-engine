#ifndef XEN_MACHINE
#define XEN_MACHINE

#include <string>

#include "Xentu.h"

namespace xen
{
	class XentuMachine
	{
		public:
			XentuMachine(const size_t argc, const char *argv[]);
			~XentuMachine();
			virtual int run();
			virtual void set_global(const std::string name, const std::string value);
		
		public:
			const size_t arg_count;
			const char* arg_values[MAX_ARGV];
	};
}

#endif