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
	class XentuLuaMachineInterop
	{
		public:
			XentuLuaMachineInterop(lua_State* L);
			~XentuLuaMachineInterop();

			int test(lua_State* L);
			int game_create_window(lua_State* L);
			int game_on(lua_State* L);
			int game_trigger(lua_State* L);
			int game_run(lua_State* L);
			int game_exit(lua_State* L);
			int geometry_create_rect(lua_State* L);
			int assets_mount(lua_State* L);
			int assets_read_text_file(lua_State* L);
			int assets_load_texture(lua_State* L);
			int assets_load_font(lua_State* L);
			int assets_create_textbox(lua_State* L);
			int renderer_begin(lua_State* L);
			int renderer_present(lua_State* L);
			int renderer_draw_texture(lua_State* L);
			int renderer_draw_sub_texture(lua_State* L);
			int renderer_draw_textbox(lua_State* L);
			int renderer_set_background(lua_State* L);
			int config_get_str(lua_State* L);
			int config_get_str2(lua_State* L);
			int config_get_bool(lua_State* L);
			int config_get_bool2(lua_State* L);
			int config_get_int(lua_State* L);
			int config_get_int2(lua_State* L);
			int textbox_set_text(lua_State* L);
			int keyboard_key_down(lua_State* L);


		public:
			//Class Constants
			static const char className[];

			// List of class properties that one can set/get from Lua
			static const Luna<XentuLuaMachineInterop>::PropertyType properties[];

			// List of class methods to make available in Lua
			static const Luna<XentuLuaMachineInterop>::FunctionType methods[];
	};
}


#endif