#ifndef XEN_STARTUP_LUA
#define XEN_STARTUP_LUA

#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX g_

#include <iostream>
#include <string>
#include "incbin/incbin.h"


// tell incbin which files to include when building.
INCBIN(LuaStartup, "lua/startup.lua");
INCBIN(LuaClasses, "lua/classes.lua");
INCBIN(LuaBeforeInit, "lua/before_init.lua");


// convert the raw data into standard strings. The _size is important as the raw data is null terminated.
const std::string m_xen_startup_lua(reinterpret_cast<const char*>(g_LuaStartup_data), g_LuaStartup_size);
const std::string m_xen_startup_lua_classes(reinterpret_cast<const char*>(g_LuaClasses_data), g_LuaClasses_size);
const std::string m_xen_startup_lua_before_init(reinterpret_cast<const char*>(g_LuaBeforeInit_data), g_LuaBeforeInit_size);


#endif