#ifndef XEN_MACHINE
#define XEN_MACHINE

#include <string>

namespace xen
{
	class XentuMachine
	{
		public:
			XentuMachine();
			~XentuMachine();
			virtual int run();
			virtual void set_global(const std::string name, const std::string value);
	};
}

#endif