#include <stdio.h>
#include <unordered_map>
#include <string>
#include <luna/luna.hpp>

#include "../Xentu.h"
#include "../XentuConfig.h"
#include "../XentuMachine.h"
#include "../fs/XenVirtualFileSystem.h"
#include "XentuLuaMachine.h"
#include "XentuLuaMachineInterop.h"
#include "XentuLuaMachineScript.h"

namespace xen
{
	XentuLuaMachine::XentuLuaMachine(int argc, char *argv[], XentuConfig* config)
	:	XentuMachine::XentuMachine(argc, argv, config)
	{
		// keep a pointer to this instance.
		if (instance != nullptr) {
			printf("Error, tried to create more than one XentuLuaMachine!");
			exit(111);
		}
		instance = this;

		L = luaL_newstate();
		luaL_openlibs(L);

		// init any classes with luna.
		Luna<xen::XentuLuaMachineInterop>::Register(L, false);

		// load in our custom import loader.
		luaL_dostring(L, xen_lua_script_init);
		XEN_LOG("- Created XentuLuaMachine\n");
	}


	XentuLuaMachine* XentuLuaMachine::instance = 0;
	XentuLuaMachine* XentuLuaMachine::GetInstance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}


	int XentuLuaMachine::Init()
	{
		XEN_LOG("- Started LuaMachine.\n");

		// load some lua code.
		auto config = this->GetConfig();
		std::string lua_code = vfs_get_global()->ReadAllText(config->entry_point) + "\r\n";

		// run the lua code.
		auto ret_startup = luaL_dostring(L, lua_code.c_str());
		if (ret_startup != LUA_OK) {
			XEN_LOG("- Error, One of the startup Lua components failed: ");
			XEN_LOG(lua_tostring(L, -1));
			XEN_LOG("\n");
			lua_pop(L, 1); // pop error message
			return 0;
		}

		// finish running.
		return 0;
	}


	int XentuLuaMachine::Trigger(const std::string event_name)
	{
		int functionRef = this->callbacks[event_name];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_call(L, 0, 0);
		}
		return 1;
	}


	int XentuLuaMachine::Trigger(const string event_name, const string arg0)
	{
		int functionRef = this->callbacks[event_name];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushstring(L, arg0.c_str());
			lua_call(L, 1, 0);
		}
		return 1;
	}


	int XentuLuaMachine::Trigger(const string event_name, const int arg0)
	{
		int functionRef = this->callbacks[event_name];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushinteger(L, arg0);
			lua_call(L, 1, 0);
		}
		return 1;
	}


	
	int XentuLuaMachine::Trigger(const string event_name, const float arg0)
	{
		int functionRef = this->callbacks[event_name];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushnumber(L, arg0);
			lua_call(L, 1, 0);
		}
		return 1;
	}


	int XentuLuaMachine::On(const std::string event_name, const int callback_ref)
	{
		this->callbacks.insert(std::make_pair(event_name, callback_ref));
		return 1;
	}
	
		
	XentuLuaMachine::~XentuLuaMachine()
	{
		lua_close(L);
		instance = nullptr;
		XEN_LOG("- Destroyed XentuLuaMachine\n");
	}
}