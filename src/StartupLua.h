#ifndef XEN_STARTUP_LUA
#define XEN_STARTUP_LUA

#include <iostream>
#include <string>


struct Res {
	const char *data;
	size_t size;
};


extern "C" Res LUA_STARTUP(void);
extern "C" Res LUA_CLASSES(void);
extern "C" Res LUA_BEFORE_INIT(void);


Res lua_startup = LUA_STARTUP(); // use *.data, *.size
Res lua_classes = LUA_CLASSES();
Res lua_before_init = LUA_BEFORE_INIT();


// convert the raw data into standard strings. The _size is important as the raw data is null terminated.
const std::string m_xen_startup_lua(lua_startup.data, lua_startup.size);
const std::string m_xen_startup_lua_classes(lua_classes.data, lua_classes.size);
const std::string m_xen_startup_lua_before_init(lua_before_init.data, lua_before_init.size);


#endif