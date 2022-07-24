#ifndef XEN_CONFIG_CPP
#define XEN_CONFIG_CPP

#include <string>

#include "Xentu.h"
#include "XentuConfig.h"

using namespace std;

namespace xen
{
	XentuConfig::XentuConfig(const char* json)
	:m_language("python")
	{
		m_data = nlohmann::json::parse(json);
		try {
			auto m_game = m_data.at("game");
			std::string m_title = m_game.at("title");
			printf("Title=%s", m_title.c_str());
		}
		catch (std::exception e)
		{
			printf("Error! %s", e.what());
			//
		}
	}


	const string XentuConfig::GetSetting(const string group, const string name, const string default_value)
	{
		try {
			auto m_group = this->m_data.at("game");
			return m_group.at("title");
		}
		catch (std::exception e)
		{
			XEN_LOG("Error! %s", e.what());
		}
		return "";
	}

	
	XentuConfig::~XentuConfig()
	{
		
	}
}

#endif