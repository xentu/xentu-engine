#ifndef XEN_KEYBOARD_MANAGER_HPP
#define XEN_KEYBOARD_MANAGER_HPP

#include <GLFW3/glfw3.h>
#include <map>
#include <luna/luna.hpp>

namespace xen
{
	/// <summary>
	/// Handles input recording for a connected keyboard.
	/// </summary>
	class KeyboardManager
	{
	public:
		KeyboardManager(lua_State* L);
		~KeyboardManager();
		void initialize(GLFWwindow* window);

		/// <summary>
		/// Check weather a key is pressed.
		/// </summary>
		int lua_key_down(lua_State* L);

		/// <summary>
		/// Check weather a key was pressed the last time it was checked.
		/// </summary>
		int lua_key_clicked(lua_State* L);


		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<KeyboardManager>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<KeyboardManager>::FunctionType methods[];
	
	private:
		GLFWwindow* m_window;
	};
}

#endif