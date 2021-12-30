#ifndef XEN_CONFIGURATION_CPP
#define XEN_CONFIGURATION_CPP

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <fstream>

#include <toml/toml.hpp>
#include "Configuration.h"
#include "../utilities/Advisor.h"

namespace xen
{
	Configuration::Configuration() :
		m_game_title("My Awesome Game!"),
		m_screen_width(800),
		m_screen_height(600),
		m_update_frequency(30),
		m_viewport_width(800),
		m_viewport_height(600),
		m_vsync(true),
		m_fullscreen(false),
		m_mode(0)
	{ }


	Configuration* Configuration::parse_file(std::string filename)
	{
		// TODO: validate this, on error it can cause a segmentation fault.
		Configuration* config = new Configuration();
		
		/* try to read the specified file. */
		std::ifstream ifs(filename);
		toml::ParseResult pr = toml::parse(ifs);

		if (!pr.valid()) {
			Advisor::throwError(pr.errorReason);
			return config;
		}

		const toml::Value& v = pr.value;

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
}

#endif