#ifndef XEN_LUA_MACHINE_CPP
#define XEN_LUA_MACHINE_CPP

#include <stdio.h>
#include "luna/luna.hpp"

#include "XentuLuaMachine.h"
#include "XentuLuaMachineInterop.h"
#include "XentuLuaMachineScript.h"
#include "../fs/XenVirtualFileSystem.h"

namespace xen
{
	XentuLuaMachine::XentuLuaMachine(int argc, char *argv[], const XentuConfig* config)
	:	XentuMachine::XentuMachine(argc, argv, config)
	{
		// keep a pointer to this instance.
		if (luaMachine != nullptr) {
			printf("Error, tried to create more than one XentuLuaMachine!");
			exit(111);
		}
		luaMachine = this;

		L = luaL_newstate();
		luaL_openlibs(L);

		// init any classes with luna.
		Luna<xen::XentuLuaGame>::Register(L, false);
		Luna<xen::XentuLuaAssets>::Register(L, false);

		// load in our custom import loader.
		luaL_dostring(L, xen_lua_script_init);

		
		XEN_LOG("\nCreated XentuLuaMachine");
	}


	int XentuLuaMachine::init(const std::string entry_point)
	{
		XEN_LOG("Lua machine started!\n");

		// load some lua code.
		std::string lua_code = vfs_get_global()->ReadAllText(entry_point) + "\r\n";

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


	int XentuLuaMachine::trigger(const std::string event_name)
	{
		int functionRef = this->callbacks[event_name];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_call(L, 0, 0);
		}
		return 1;
	}


	int XentuLuaMachine::on(const std::string event_name, const int callback_ref)
	{
		this->callbacks.insert(std::make_pair(event_name, callback_ref));
		return 1;
	}
	
		
	XentuLuaMachine::~XentuLuaMachine()
	{
		lua_close(L);
		luaMachine = nullptr;
		XEN_LOG("\nDestroyed XentuLuaMachine");
	}
}

#endif