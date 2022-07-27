#ifndef XEN_LUA_MACHINE_INTEROP_CPP
#define XEN_LUA_MACHINE_INTEROP_CPP


#include "luna/luna.hpp"

#include "XentuLuaMachineInterop.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	XentuLuaGame::XentuLuaGame(lua_State* L)
	{
		printf("XentuLuaGame::XentuLuaGame called");
	}


	XentuLuaGame::~XentuLuaGame()
	{
		printf("XentuLuaGame::~XentuLuaGame called\n");
	}


	int XentuLuaGame::test(lua_State* L)
	{
		printf("XentuLuaGame::test called\n");
		return 0;
	}


	const char xen::XentuLuaGame::className[] = "XentuLuaGame";

	const Luna<XentuLuaGame>::PropertyType XentuLuaGame::properties[] = {
		{0,0}
	};

	const Luna<XentuLuaGame>::FunctionType XentuLuaGame::methods[] = {
		method(XentuLuaGame, test, test),
		{0,0}
	};
}


#endif