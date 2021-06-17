#ifndef XEN_KEYBOARD_MANAGER_CPP
#define XEN_KEYBOARD_MANAGER_CPP

#include <iostream>

#include "KeyboardManager.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	int m_keys_down[GLFW_KEY_LAST];



	KeyboardManager::KeyboardManager(lua_State* L) :
		m_window(0)
	{ }



	KeyboardManager::~KeyboardManager()
	{
		std::cout << "Deleted instance of KeyboardManager." << std::endl;
	}



	void KeyboardManager::initialize(GLFWwindow* window)
	{
		m_window = window;

		for (int i = 0; i < GLFW_KEY_LAST; i++) {
			m_keys_down[i] = GLFW_RELEASE;
		}

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

		auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			

			m_keys_down[key] = action;
		};
		glfwSetKeyCallback(window, key_callback);
	}



	int KeyboardManager::lua_key_down(lua_State* L)
	{	
		int key_code = lua_tointeger(L, -1);
		bool down = glfwGetKey(m_window, key_code) == GLFW_PRESS;
		lua_pushboolean(L, down);
		return 1;
	}



	const char KeyboardManager::className[] = "KeyboardManager";



	const Luna<KeyboardManager>::PropertyType KeyboardManager::properties[] = {
		{0,0}
	};



	const Luna<KeyboardManager>::FunctionType KeyboardManager::methods[] = {
		method(KeyboardManager, key_down, lua_key_down),
		{0,0}
	};
}

#endif