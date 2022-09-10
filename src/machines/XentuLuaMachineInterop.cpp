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


	int XentuLuaGame::create_window(lua_State* L)
	{
		printf("XentuLuaGame::create_window called\n");
		return 0;
	}


	const char xen::XentuLuaGame::className[] = "XentuLuaGame";

	const Luna<XentuLuaGame>::PropertyType XentuLuaGame::properties[] = {
		{0,0}
	};

	const Luna<XentuLuaGame>::FunctionType XentuLuaGame::methods[] = {
		method(XentuLuaGame, create_window, create_window),
		{0,0}
	};


	// ---------------


	XentuLuaAssets::XentuLuaAssets(lua_State* L)
	{
		printf("XentuLuaAssets::XentuLuaAssets called");
	}


	XentuLuaAssets::~XentuLuaAssets()
	{
		printf("XentuLuaAssets::~XentuLuaAssets called\n");
	}


	int XentuLuaAssets::load_texture(lua_State* L)
	{
		printf("XentuLuaAssets::load_texture called\n");
		return 0;
	}


	const char xen::XentuLuaAssets::className[] = "XentuLuaAssets";

	const Luna<XentuLuaAssets>::PropertyType XentuLuaAssets::properties[] = {
		{0,0}
	};

	const Luna<XentuLuaAssets>::FunctionType XentuLuaAssets::methods[] = {
		method(XentuLuaAssets, load_texture, load_texture),
		{0,0}
	};
}


#endif