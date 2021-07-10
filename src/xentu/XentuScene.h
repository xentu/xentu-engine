#ifndef XEN_SCENE_HPP
#define XEN_SCENE_HPP

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
		/// Allows scripts in lua to subscribe to events that can be fired using the Trigger() method.
		/// </summary>
		/// <param name="L">Pointer to the running lua_State.</param>
		/// <returns>Exit code, 0 unless something went wrong.</returns>
		int lua_on(lua_State* L);	

		/// <summary>
		/// Allows scripts in Lua to trigger events.
		/// </summary>
		int lua_trigger(lua_State* L);

		int lua_get_name(lua_State* L);
		int lua_set_name(lua_State* L);

        //Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<XentuScene>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<XentuScene>::FunctionType methods[];

    private:
		std::string m_name;
        /* Store a hashmap of assigned event callbacks. */
		std::unordered_map<std::string, int> m_callbacks;
    };
}

#endif