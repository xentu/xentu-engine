#ifndef XEN_CONFIGURATION_CPP
#define XEN_CONFIGURATION_CPP

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <fstream>

#include <toml/toml.hpp>
#include "Configuration.h"

namespace xen
{
	Configuration::Configuration() :
		m_screen_width(800),
		m_screen_height(600),
		m_viewport_width(800),
		m_viewport_height(600)
	{ }


	Configuration* Configuration::parse_file(std::string filename)
	{
		Configuration* config = new Configuration();
		
		/* try to read the specified file. */
		std::ifstream ifs(filename);
		toml::ParseResult pr = toml::parse(ifs);

		const toml::Value& v = pr.value;

		if (!pr.valid()) {
			std::cout << pr.errorReason << std::endl;
			return config;
		}

		if (v.find("general.game_title")->is<std::string>()) {
			config->m_game_title = v.find("general.game_title")->as<std::string>();
		}

		if (v.find("general.screen_width")->is<int>()) {
			config->m_screen_width = v.find("general.screen_width")->as<int>();
		}

		if (v.find("general.screen_height")->is<int>()) {
			config->m_screen_height = v.find("general.screen_height")->as<int>();
		}

		if (v.find("general.update_frequency")->is<int>()) {
			config->m_update_frequency = v.find("general.update_frequency")->as<int>();
		}

		if (v.find("viewport.width")->is<int>()) {
			config->m_viewport_width = v.find("viewport.width")->as<int>();
		}

		if (v.find("viewport.height")->is<int>()) {
			config->m_viewport_height = v.find("viewport.height")->as<int>();
		}

		return config;
	}
}

#endif