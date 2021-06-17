#ifndef XEN_MOUSE_MANAGER_HPP
#define XEN_MOUSE_MANAGER_HPP

#include <GLFW3/glfw3.h>
#include <map>
#include <luna/luna.hpp>

namespace xen
{
	/// <summary>
	/// Handles input recording for a connected mouse.
	/// </summary>
	class MouseManager
	{
    public:
        MouseManager(lua_State* L);
		~MouseManager();
		void initialize(GLFWwindow* window);
        int lua_button_down(lua_State* L);
		int lua_get_mouse_x(lua_State* L);
		int lua_get_mouse_y(lua_State* L);

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<MouseManager>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<MouseManager>::FunctionType methods[];

    private:
        GLFWwindow* m_window;
    };
}


#endif