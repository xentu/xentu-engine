#ifndef XEN_LUA_MACHINE
#define XEN_LUA_MACHINE

#include <unordered_map>
#include <string>

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
			XentuLuaMachine(const int argc, const char *argv[], const XentuConfig* config);
			~XentuLuaMachine();
			int run(const std::string entry_point);
			int trigger(const std::string event_name);
			int on(const std::string event_name, const int callback_ref);
		
		private:
			lua_State* L;
			std::unordered_map<std::string, int> callbacks;
	};


	// when using the lua machine, this will be used to bridge communication
	// between the class instance and lua.
	static XentuLuaMachine* luaMachine = nullptr;
}

#endif