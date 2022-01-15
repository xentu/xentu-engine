#ifndef XEN_SCENE_HPP
#define XEN_SCENE_HPP

#include <string>
#include <unordered_map>
#include <luna/luna.hpp>


namespace xen
{
	class XentuScene
	{
	public:
		/// <summary>
		/// Creates a new xentu instance.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		XentuScene(lua_State* L);
		~XentuScene(void);

        /// <summary>
		/// Trigger a named event.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="callbackName">The name of the event to trigger.</param>
		void trigger(lua_State* L, std::string callbackName);

		/// <summary>
		/// Trigger a named event, and pass a single integer parameter.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="callbackName">The name of the event to trigger.</param>
		/// <param name="data">The data to passs with the event.</param>
		void trigger_integer(lua_State* L, std::string callbackName, lua_Integer data);

		/// <summary>
		/// Trigger a named event, and pass a single number (float etc...) parameter.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <param name="callbackName">The name of the event to trigger.</param>
		/// <param name="data">The data to passs with the event.</param>
		void trigger_number(lua_State* L, std::string callbackName, lua_Number data);

        /// <summary>
		/// Allows scripts in lua to subscribe to events that can be fired using the Trigger() method.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <returns>Exit code, 0 unless something went wrong.</returns>
		int lua_on(lua_State* L);	

		/// <summary>
		/// Allows scripts in Lua to trigger events.
		/// </summary>
		int lua_trigger(lua_State* L);

		/// <summary>
		/// Get the name of this scene.
		/// </summary>
		int lua_get_name(lua_State* L);

		/// <summary>
		/// Set the name of this scene.
		/// </summary>
		int lua_set_name(lua_State* L);

        //Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<XentuScene>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<XentuScene>::FunctionType methods[];

		bool m_initialized;
    private:
		std::string m_name;
        /* Store a hashmap of assigned event callbacks. */
		std::unordered_map<std::string, int> m_callbacks;
    };
}

#endif