#include <iostream>
#include <fstream>

#include "LuaMachine.h"
#include "LuaMachineInterop.h"
#include "LuaMachineScripts.h"

//#include "../../../lib/luna.hpp"
#include "../assets/DataManager.h"

namespace xen
{
	LuaMachine::LuaMachine()
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
		for (auto & i : xen_lua_script_init) {
			luaL_dostring(L, i);
		}
	}

	LuaMachine::~LuaMachine() {
		instance = nullptr;
	}

	LuaMachine* LuaMachine::instance = nullptr;
	LuaMachine* LuaMachine::GetInstance() {
		if (instance == nullptr) {
			XEN_ERROR("Error, called LuaMachine::GetInstance() before initialization!");
			exit(123);
		}
		return instance;
	}

	int LuaMachine::Init() {

		// load some lua code.
		auto config = this->GetConfig();

		auto entryPoint = "resources/" + config->entry_point;

		// std::ifstream file(entryPoint, std::ios::binary);
		// std::ostringstream buffer;
		// buffer << file.rdbuf();
		// std::string lua_code = buffer.str() + "\r\ngame.run()\n\n";

		std::string lua_code = DataManager::GetInstance()->ReadAllText(entryPoint) + "\r\ngame.run()\n\n";

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

	int LuaMachine::Trigger(const string event_name) {
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, it->second);
			lua_call(L, 0, 0);
		}
		return 1;
	}

	int LuaMachine::Trigger(const string event_name, const string arg0) {
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, it->second);
			lua_pushstring(L, arg0.c_str());
			lua_call(L, 1, 0);
		}
		return 1;
	}

	int LuaMachine::Trigger(const string event_name, const int arg0) {
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, it->second);
			lua_pushinteger(L, arg0);
			lua_call(L, 1, 0);
		}
		return 1;
	}

	int LuaMachine::Trigger(const string event_name, const float arg0) {
		auto its = this->callbacks.equal_range(event_name);
		for (auto it = its.first; it != its.second; ++it) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, it->second);
			lua_pushnumber(L, arg0);
			lua_call(L, 1, 0);
		}
		return 1;
	}

	int LuaMachine::On(const string event_name, const int callback_ref) {
		XEN_DEBUG("> LUA: On event %s - %d\n", event_name.c_str(), callback_ref);
		//this->callbacks.insert(std::make_pair(event_name, callback_ref));
		auto pair = std::make_pair(event_name, callback_ref);
		this->callbacks.insert(pair);
		return 1;
	}
}