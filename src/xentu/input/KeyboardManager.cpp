#ifndef XEN_KEYBOARD_MANAGER_CPP
#define XEN_KEYBOARD_MANAGER_CPP
#define GLFW_INCLUDE_NONE

#include <iostream>

#include "KeyboardManager.h"
#include "XentuGame.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	int m_keys_down[GLFW_KEY_LAST];
	int m_key_presses[GLFW_KEY_LAST];


	KeyboardManager::KeyboardManager(lua_State* L) :
		m_window(0)
	{ }



	KeyboardManager::~KeyboardManager()
	{
		Advisor::logInfo("Deleted instance of KeyboardManager.");
	}



	void KeyboardManager::initialize(GLFWwindow* window)
	{
		m_window = window;

		for (int i = 0; i < GLFW_KEY_LAST; i++) {
			m_keys_down[i] = GLFW_RELEASE;
			m_key_presses[i] = 0;
		}

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

		auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			int old_action = m_keys_down[key];

			if (old_action == GLFW_PRESS && action == GLFW_RELEASE) {
				m_key_presses[key] = m_key_presses[key] + 1;
			}

			m_keys_down[key] = action;
		};
		glfwSetKeyCallback(window, key_callback);
	}



	int KeyboardManager::lua_key_down(lua_State* L)
	{	
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		int key_code = lua_tointeger(L, -1);
		bool down = glfwGetKey(m_window, key_code) == GLFW_PRESS;
		lua_pushboolean(L, down);
		return 1;
	}


	int KeyboardManager::lua_key_clicked(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		int key_code = lua_tointeger(L, -1);
		bool down = m_key_presses[key_code];
		lua_pushboolean(L, down);
		m_key_presses[key_code] = 0;
		return 1;
	}


	const char KeyboardManager::className[] = "KeyboardManager";



	const Luna<KeyboardManager>::PropertyType KeyboardManager::properties[] = {
		{0,0}
	};



	const Luna<KeyboardManager>::FunctionType KeyboardManager::methods[] = {
		method(KeyboardManager, key_down, lua_key_down),
		method(KeyboardManager, key_clicked, lua_key_clicked),
		{0,0}
	};
}

#endif