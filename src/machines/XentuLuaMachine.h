#ifndef XEN_LUA_MACHINE
#define XEN_LUA_MACHINE

#include "../XentuMachine.h"

namespace xen
{
	class XentuLuaMachine : public XentuMachine
	{
		public:
				XentuLuaMachine();
				~XentuLuaMachine();
				void set_global(const std::string name, const std::string value);
	};
}

#endif