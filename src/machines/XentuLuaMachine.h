#ifndef XEN_LUA_MACHINE
#define XEN_LUA_MACHINE

#include "../XentuMachine.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace xen
{
	class XentuLuaMachine : public XentuMachine
	{
		public:
			XentuLuaMachine(const int argc, const char *argv[]);
			~XentuLuaMachine();
			int run(const std::string entry_point);
		
		private:
			lua_State* L;
	};
}

#endif