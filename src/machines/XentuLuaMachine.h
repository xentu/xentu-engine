#ifndef XEN_LUA_MACHINE
#define XEN_LUA_MACHINE

#include "../XentuMachine.h"

namespace xen
{
	class XentuLuaMachine : public XentuMachine
	{
		public:
				XentuLuaMachine(const int argc, const char *argv[]);
				~XentuLuaMachine();
				void set_global(const std::string name, const std::string value);
	};
}

#endif