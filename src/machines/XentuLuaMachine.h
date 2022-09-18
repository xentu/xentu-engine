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
			XentuLuaMachine(int argc, char *argv[], XentuConfig* config);
			~XentuLuaMachine();
			static XentuLuaMachine* GetInstance();
			int Init();
			int Trigger(const std::string event_name);
			int On(const std::string event_name, const int callback_ref);
		
		private:
			lua_State* L;
			std::unordered_map<std::string, int> callbacks;
			static XentuLuaMachine* instance;
	};
}

#endif