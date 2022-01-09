#ifndef XEN_CONFIGURATION_CPP
#define XEN_CONFIGURATION_CPP

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <fstream>

#include <luna/luna.hpp>
#include <toml/toml.hpp>
#include "Configuration.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	Configuration::Configuration(lua_State* L) :
		m_game_title("My Awesome Game!"),
		m_screen_width(800),
		m_screen_height(600),
		m_update_frequency(30),
		m_viewport_width(800),
		m_viewport_height(600),
		m_vsync(true),
		m_fullscreen(false),
		m_mode(0),
		m_value(toml::Value(false))
	{ }


	Configuration* Configuration::parse_file(lua_State* L, std::string filename)
	{
		// TODO: validate this, on error it can cause a segmentation fault.
		Configuration* config = new Configuration(L);
		
		/* try to read the specified file. */
		std::ifstream ifs(filename);
		toml::ParseResult pr = toml::parse(ifs);

		if (!pr.valid()) {
			Advisor::throwError(pr.errorReason);
			return config;
		}

		const toml::Value& v = pr.value;
		config->m_value = v;

		if (v.has("general.game_title") && v.find("general.game_title")->is<std::string>()) {
			config->m_game_title = v.find("general.game_title")->as<std::string>();
		}

		if (v.has("general.screen_width") && v.find("general.screen_width")->is<int>()) {
			config->m_screen_width = v.find("general.screen_width")->as<int>();
		}

		if (v.has("general.screen_height") && v.find("general.screen_height")->is<int>()) {
			config->m_screen_height = v.find("general.screen_height")->as<int>();
		}

		if (v.has("general.update_frequency") && v.find("general.update_frequency")->is<int>()) {
			config->m_update_frequency = v.find("general.update_frequency")->as<int>();
		}

		if (v.has("general.vsync") && v.find("general.vsync")->is<bool>()) {
			config->m_vsync = v.find("general.vsync")->as<bool>();
		}

		if (v.has("viewport.width") && v.find("viewport.width")->is<int>()) {
			config->m_viewport_width = v.find("viewport.width")->as<int>();
		}

		if (v.has("viewport.height") && v.find("viewport.height")->is<int>()) {
			config->m_viewport_height = v.find("viewport.height")->as<int>();
		}

		if (v.has("general.fullscreen") && v.find("general.fullscreen")->is<bool>()) {
			config->m_fullscreen = v.find("general.fullscreen")->as<bool>();
		}

		if (v.has("viewport.mode") && v.find("viewport.mode")->is<int>()) {
			config->m_mode = v.find("viewport.mode")->as<int>();
		}

		return config;
	}


	int Configuration::lua_get_string(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const std::string key = lua_tostring(L, -1);
		if (m_value.has(key) && m_value.find(key)->is<std::string>()) {
			const std::string value = m_value.find(key)->as<std::string>();
			lua_pushstring(L, value.c_str());
			return 1;
		}
		return 0;
	}


	int Configuration::lua_get_integer(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const std::string key = lua_tostring(L, -1);
		if (m_value.has(key) && m_value.find(key)->is<int>()) {
			const int value = m_value.find(key)->as<int>();
			lua_pushinteger(L, value);
			return 1;
		}
		return 0;
	}


	const char Configuration::className[] = "Configuration";


	const Luna<Configuration>::PropertyType Configuration::properties[] = {
		{0,0}
	};


	const Luna<Configuration>::FunctionType Configuration::methods[] = {
		method(Configuration, get_string, lua_get_string),
		method(Configuration, get_integer, lua_get_integer),
		{0,0}
	};
}

#endif