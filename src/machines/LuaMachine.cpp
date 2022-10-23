#include <stdio.h>
#include <unordered_map>
#include <string>
#include <luna/luna.hpp>

#include "../Globals.h"
#include "../Config.h"
#include "../Machine.h"
#include "../vfs/XenVirtualFileSystem.h"
#include "LuaMachine.h"
#include "LuaMachineInterop.h"
#include "LuaMachineScript.h"

namespace xen
{
	LuaMachine::LuaMachine(int argc, char *argv[], Config* config)
	:	Machine::Machine(argc, argv, config)
	{
		// keep a pointer to this instance.
		if (instance != nullptr) {
			XEN_ERROR("> Error, tried to create more than one LuaMachine!");
			exit(111);
		}
		instance = this;

		L = luaL_newstate();
		luaL_openlibs(L);

		// init any classes with luna.
		Luna<xen::XentuLuaMachineInterop>::Register(L, false);

		// load in our custom import loader.
		luaL_dostring(L, xen_lua_script_init);
	}


	LuaMachine* LuaMachine::instance = 0;
	LuaMachine* LuaMachine::GetInstance()
	{
		if (instance == 0)
		{
			XEN_ERROR("Error, called LuaMachine::GetInstance() before initialization!");
			exit(123);
		}
		return instance;
	}


	int LuaMachine::Init()
	{
		// load some lua code.
		auto config = this->GetConfig();
		std::string lua_code = vfs_get_global()->ReadAllText(config->entry_point) + "\r\n";

		// run the lua code.
		const int ret_startup = luaL_dostring(L, lua_code.c_str());
		if (ret_startup != LUA_OK) {
			XEN_ERROR("> Error, One of the startup Lua components failed: ");
			XEN_ERROR(lua_tostring(L, -1));
			XEN_ERROR("\n");
			lua_pop(L, 1); // pop error message
			return 0;
		}

		// finish running.
		return 0;
	}


	int LuaMachine::Trigger(const std::string event_name)
	{
		int functionRef = this->callbacks[event_name];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_call(L, 0, 0);
		}
		return 1;
	}


	int LuaMachine::Trigger(const string event_name, const string arg0)
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


	int LuaMachine::Trigger(const string event_name, const int arg0)
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


	
	int LuaMachine::Trigger(const string event_name, const float arg0)
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


	int LuaMachine::On(const std::string event_name, const int callback_ref)
	{
		this->callbacks.insert(std::make_pair(event_name, callback_ref));
		return 1;
	}
	
		
	LuaMachine::~LuaMachine()
	{
		lua_close(L);
		instance = nullptr;
	}
}