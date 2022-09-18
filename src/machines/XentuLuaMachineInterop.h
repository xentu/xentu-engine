#ifndef XEN_LUA_MACHINE_INTEROP_H
#define XEN_LUA_MACHINE_INTEROP_H


#include "luna/luna.hpp"

#include "../XentuMachine.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


namespace xen
{
	class XentuLuaGame
	{
		public:
			XentuLuaGame(lua_State* L);
			~XentuLuaGame();
			int CreateWindow(lua_State* L);

		public:
			//Class Constants
			static const char className[];

			// List of class properties that one can set/get from Lua
			static const Luna<XentuLuaGame>::PropertyType properties[];

			// List of class methods to make available in Lua
			static const Luna<XentuLuaGame>::FunctionType methods[];
	};


	class XentuLuaAssets
	{
		public:
			XentuLuaAssets(lua_State* L);
			~XentuLuaAssets();
			int load_texture(lua_State* L);

		public:
			//Class Constants
			static const char className[];

			// List of class properties that one can set/get from Lua
			static const Luna<XentuLuaAssets>::PropertyType properties[];

			// List of class methods to make available in Lua
			static const Luna<XentuLuaAssets>::FunctionType methods[];
	};
}


#endif