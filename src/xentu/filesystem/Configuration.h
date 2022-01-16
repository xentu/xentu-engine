#ifndef XEN_CONFIGURATION_HPP
#define XEN_CONFIGURATION_HPP

#define _CRT_SECURE_NO_WARNINGS

#include <toml/toml.hpp>
#include <string>

namespace xen
{
	class Configuration
	{
	public:
		Configuration(lua_State* L);
		static Configuration* parse_file(lua_State* L, std::string filename);

		// todo: lots of variables here that should not be public.
		std::string m_game_title;
		std::string m_game_author;
		unsigned int m_screen_width;
		unsigned int m_screen_height;
		unsigned int m_update_frequency;
		unsigned int m_viewport_width;
		unsigned int m_viewport_height;
		bool m_vsync;
		bool m_fullscreen;

		/* 0=none, 1=centre, 2=stretch */
		int m_mode;


		int lua_get_string(lua_State* L);
		int lua_get_integer(lua_State* L);


		//Class Constants
		static const char className[];


		// List of class properties that one can set/get from Lua
		static const Luna<Configuration>::PropertyType properties[];


		// List of class methods to make available in Lua
		static const Luna<Configuration>::FunctionType methods[];

	private:
		toml::Value m_value;
	};
}

#endif