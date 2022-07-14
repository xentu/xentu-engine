#ifndef XEN_LUA_MACHINE_CPP
#define XEN_LUA_MACHINE_CPP

#include <stdio.h>
#include "XentuLuaMachine.h"

namespace xen
{
	XentuLuaMachine::XentuLuaMachine(const int argc, const char *argv[])
	:	XentuMachine::XentuMachine(argc, argv)
	{
		XEN_LOG("\nCreated XentuLuaMachine");
		L = luaL_newstate();
		luaL_openlibs(L);
	}


	int XentuLuaMachine::run(const std::string entry_point)
	{
		XEN_LOG("Lua machine started!\n");

		// load some lua code.
		std::string lua_code = read_text_file("/test.lua") + "\r\n";

		// run the lua code.
		auto ret_startup = luaL_dostring(L, lua_code.c_str());
		if (ret_startup != LUA_OK) {
			XEN_LOG("One of the startup Lua components failed: ");
			XEN_LOG(lua_tostring(L, -1));
			lua_pop(L, 1); // pop error message
			return 0;
		}

		// finish running.
		return 0;
	}
	
		
	XentuLuaMachine::~XentuLuaMachine()
	{
		lua_close(L);
		XEN_LOG("\nDestroyed XentuLuaMachine");
	}
}

#endif