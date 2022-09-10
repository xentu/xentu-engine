#ifndef XEN_LUA_MACHINE_SCRIPTS_H
#define XEN_LUA_MACHINE_SCRIPTS_H
/**
 * This file holds the lua code to be executed when a lua engine is loaded
 * by the user. The file does the following:
 * 
 */
namespace xen { const char * xen_lua_script_init = R"(

game = XentuLuaGame()
assets = XentuLuaAssets()

geometry = {}
geometry.create_rect = function(x, y, width, height)
	return { x, y, width, height }
end

)"; }
#endif