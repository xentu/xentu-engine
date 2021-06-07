#ifndef XEN_INPUT_MANAGER_HPP
#define XEN_INPUT_MANAGER_HPP

#include <GLFW3/glfw3.h>
#include <map>
#include <luna/luna.hpp>

namespace xen
{
	/// <summary>
	/// Handles input recording.
	/// </summary>
	class InputManager
	{
	public:
		InputManager(lua_State* L);
		~InputManager();
		void initialize(GLFWwindow* window);

		/// <summary>
		/// Check weather a key is pressed.
		/// </summary>
		int key_down(lua_State* L);

		/// <summary>
		/// Check weather a key was released after being pressed (i.e. clicked).
		/// </summary>
		int key_clicked(lua_State* L);

		int mouse_down(lua_State* L);

		int get_mouse_x(lua_State* L);
		int get_mouse_y(lua_State* L);

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<InputManager>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<InputManager>::FunctionType methods[];
	
	private:
		GLFWwindow* m_window;
	};
}

#endif