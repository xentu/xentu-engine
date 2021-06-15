#ifndef XEN_INPUT_MANAGER_CPP
#define XEN_INPUT_MANAGER_CPP

#include <iostream>

#include "InputManager.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	int m_keys_down[GLFW_KEY_LAST];
	bool m_keys_clicked[GLFW_KEY_LAST];
	int m_last_mouse_x;
	int m_last_mouse_y;


	InputManager::InputManager(lua_State* L) :
		m_window(0)
	{ }



	InputManager::~InputManager()
	{
		std::cout << "Deleted instance of InputManager." << std::endl;
	}



	void InputManager::initialize(GLFWwindow* window)
	{
		m_window = window;

		for (int i = 0; i < GLFW_KEY_LAST; i++) {
			m_keys_down[i] = GLFW_RELEASE;
			m_keys_clicked[i] = false;
		}

		m_last_mouse_x = 0;
		m_last_mouse_y = 0;

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

		auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			bool was_down = m_keys_down[key] == GLFW_PRESS || m_keys_down[key] == GLFW_REPEAT;
			if (was_down && action == GLFW_RELEASE)
			{
				m_keys_clicked[key] = true;
			}
			m_keys_down[key] = action;
		};
		glfwSetKeyCallback(window, key_callback);


		auto cursor_position_callback = [](GLFWwindow* window, double xpos, double ypos)
		{
			m_last_mouse_x = static_cast<int>(xpos);
			m_last_mouse_y = static_cast<int>(ypos);
			//std::cout << "Y: " << m_last_mouse_y << std::endl;
		};
		glfwSetCursorPosCallback(window, cursor_position_callback);


		/* auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
				popup_menu();
		};
		glfwSetMouseButtonCallback(window, mouse_button_callback); */
	}



	int InputManager::lua_key_down(lua_State* L)
	{	
		int key_code = lua_tointeger(L, -1);
		bool down = glfwGetKey(m_window, key_code) == GLFW_PRESS;
		lua_pushboolean(L, down);
		return 1;
	}



	int InputManager::lua_key_clicked(lua_State* L)
	{
		int key_code = lua_tointeger(L, -1);
		bool clicked = m_keys_clicked[key_code];
		m_keys_clicked[key_code] = false;
		lua_pushboolean(L, clicked);
		return 1;
	}


	int InputManager::lua_mouse_down(lua_State* L)
	{
		int button = lua_tointeger(L, -1);
		int state = glfwGetMouseButton(m_window, button);
		lua_pushboolean(L, state == GLFW_PRESS);
		return 1;
	}


	int InputManager::lua_get_mouse_x(lua_State* L)
	{
		lua_pushinteger(L, m_last_mouse_x);
		return 1;
	}



	int InputManager::lua_get_mouse_y(lua_State* L)
	{
		lua_pushinteger(L, m_last_mouse_y);
		return 1;
	}



	const char InputManager::className[] = "InputManager";



	const Luna<InputManager>::PropertyType InputManager::properties[] = {
		{"mouse_x", &InputManager::lua_get_mouse_x, nullptr },
		{"mouse_y", &InputManager::lua_get_mouse_y, nullptr },
		{0,0}
	};



	const Luna<InputManager>::FunctionType InputManager::methods[] = {
		method(InputManager, key_down, lua_key_down),
		method(InputManager, key_clicked, lua_key_clicked),
		method(InputManager, mouse_down, lua_mouse_down),
		{0,0}
	};
}

#endif