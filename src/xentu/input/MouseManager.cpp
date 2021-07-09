#ifndef XEN_MOUSE_MANAGER_CPP
#define XEN_MOUSE_MANAGER_CPP
#define GLFW_INCLUDE_NONE

#include <iostream>
#include <math.h>

#include "MouseManager.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	int m_last_x;
	int m_last_y;


    MouseManager::MouseManager(lua_State* L) :
		m_window(0)
	{ }



	MouseManager::~MouseManager()
	{
		Advisor::logInfo("Deleted instance of MouseManager.");
	}



    void MouseManager::initialize(GLFWwindow* window)
    {
        m_window = window;
		m_last_x = 0;
		m_last_y = 0;
		auto cursor_position_callback = [](GLFWwindow* window, double xpos, double ypos)
		{
			m_last_x = static_cast<int>(xpos);
			m_last_y = static_cast<int>(ypos);
		};
		glfwSetCursorPosCallback(window, cursor_position_callback);
    }



    int MouseManager::lua_button_down(lua_State* L)
    {
        int button = lua_tointeger(L, -1);
		int state = glfwGetMouseButton(m_window, button);
		lua_pushboolean(L, state == GLFW_PRESS);
		return 1;
    }
	
    

    int MouseManager::lua_get_mouse_x(lua_State* L)
    {
        lua_pushinteger(L, m_last_x);
		return 1;
    }

    
    int MouseManager::lua_get_mouse_y(lua_State* L)
    {
        lua_pushinteger(L, m_last_y);
		return 1;
    }



    const char MouseManager::className[] = "MouseManager";



	const Luna<MouseManager>::PropertyType MouseManager::properties[] = {
		{"mouse_x", &MouseManager::lua_get_mouse_x, nullptr },
		{"mouse_y", &MouseManager::lua_get_mouse_y, nullptr },
		{0,0}
	};



	const Luna<MouseManager>::FunctionType MouseManager::methods[] = {
		method(MouseManager, button_down, lua_button_down),
		{0,0}
	};
}


#endif