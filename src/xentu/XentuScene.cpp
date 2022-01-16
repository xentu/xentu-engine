#ifndef XEN_SCENE_CPP
#define XEN_SCENE_CPP

#include <string>

#include "XentuScene.h"


// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}


namespace xen
{
	XentuScene::XentuScene(lua_State* L) :
		m_name("untitled"),
		m_initialized(false)
	{ }
	
    XentuScene::~XentuScene()
    {

    }


    void XentuScene::trigger(lua_State* L, std::string callbackName)
	{
		int functionRef = this->m_callbacks[callbackName];
		if (functionRef > 0)
		{
			bool is_init = callbackName == "init";

			if (is_init && this->m_initialized == true) {
				// todo: this should probably return a value to the caller.
				luaL_error(L, "The init event has already been called once.");
			}

			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_call(L, 0, 0);

			// if init is called manually, then set m_initialized
			if (is_init) {
				this->m_initialized = true;
			}
		}
	}


	void XentuScene::trigger_integer(lua_State* L, std::string callbackName, lua_Integer data)
	{
		int functionRef = this->m_callbacks[callbackName];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushinteger(L, data);
			lua_call(L, 1, 0);
		}
	}


	void XentuScene::trigger_number(lua_State* L, std::string callbackName, lua_Number data)
	{
		int functionRef = this->m_callbacks[callbackName];
		if (functionRef > 0)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, functionRef);
			lua_pushnumber(L, data);
			lua_call(L, 1, 0);
		}
	}


    int XentuScene::lua_on(lua_State* L)
	{
		if (lua_isfunction(L, -1))
		{
			int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
			std::string callbackName = lua_tostring(L, -1);
			this->m_callbacks.insert(std::make_pair(callbackName, functionRef));
			return 1;
		}

		return 0;
	}
    

	int XentuScene::lua_trigger(lua_State* L)
	{
		std::string event = lua_tostring(L, -1);

        this->trigger(L, event);
		
		return 0;
	}


	int XentuScene::lua_get_name(lua_State* L)
	{
		lua_pushstring(L, this->m_name.c_str());
		return 1;
	}


	int XentuScene::lua_set_name(lua_State* L)
	{
		this->m_name = lua_tostring(L, -1);
		return 0;
	}


    const char xen::XentuScene::className[] = "XentuScene";

	const Luna<XentuScene>::PropertyType xen::XentuScene::properties[] = {
		{"name", &XentuScene::lua_get_name, &XentuScene::lua_set_name },
		{0,0}
	};

	const Luna<XentuScene>::FunctionType xen::XentuScene::methods[] = {
		method(XentuScene, on, lua_on),
		method(XentuScene, trigger, lua_trigger),
		{0,0}
	};
}

#endif