#ifndef XEN_CONFIG_CPP
#define XEN_CONFIG_CPP

#include <string>

#include "Xentu.h"
#include "XentuConfig.h"

using namespace std;

namespace xen
{
	XentuConfig::XentuConfig(const char* json)
	{
		m_data = nlohmann::json::parse(json);
		try {
			auto m_game = m_data.at("game");

			title = m_game.at("title");
			title = this->GetSetting("game", "title", "");
			language = this->GetSetting("game", "language", "");
			entry_point = this->GetSetting("game", "entry_point", "");
			v_sync = this->GetSettingBool("game", "v_sync", true);
			fullscreen = this->GetSettingBool("game", "fullscreen", false);
			update_frequency = this->GetSettingInt("game", "update_frequency", 30);

			int win_x = this->GetSettingInt("game", "window", "x", -1);
			int win_y = this->GetSettingInt("game", "window", "y", -1);
			int win_w = this->GetSettingInt("game", "window", "width", 640);
			int win_h = this->GetSettingInt("game", "window", "height", 480);
			window = { win_x, win_y, win_w, win_h };

			string ren_engine_s = this->GetSetting("game", "renderer", "engine", "sdl2");
			XentuRenderEngine ren_engine = XentuRenderEngine::sdl2;
			if (ren_engine_s == "xna") ren_engine = XentuRenderEngine::xna;
			if (ren_engine_s == "open_tk") ren_engine = XentuRenderEngine::open_tk;
			string ren_teq_s = this->GetSetting("game", "renderer", "technique", "immediate");
			XentuRenderTechnique ren_teq = XentuRenderTechnique::immediate;
			if (ren_engine_s == "deferred") ren_teq = XentuRenderTechnique::deferred;
			bool ren_acc = this->GetSettingBool("game", "renderer", "accelerated", true);
			renderer = { ren_engine, ren_teq, ren_acc };

			printf("Entry point: %s\n", entry_point.c_str());

		}
		catch (std::exception e)
		{
			printf("Config error! %s\n", e.what());
			//
		}
	}


	const string XentuConfig::GetSetting(const string group, const string name, const string default_value)
	{
		try {
			auto m_group = m_data.at(group);
			return m_group.at(name);
		}
		catch (std::exception e)
		{
			XEN_LOG("- Config warning: GetSetting [%s.%s] missing.\n", group.c_str(), name.c_str());
		}
		return default_value;
	}
	const bool XentuConfig::GetSettingBool(const string group, const string name, const bool default_value)
	{
		try {
			auto m_group = m_data.at(group);
			return m_group.at(name);
		}
		catch (std::exception e)
		{
			XEN_LOG("- Config warning: SetSetting [%s.%s] missing.\n", group.c_str(), name.c_str());
		}
		return default_value;
	}
	const int XentuConfig::GetSettingInt(const string group, const string name, const int default_value)
	{
		try {
			auto m_group = m_data.at(group);
			return m_group.at(name);
		}
		catch (std::exception e)
		{
			XEN_LOG("- Config warning: GetSettingInt [%s.%s] missing.\n", group.c_str(), name.c_str());
		}
		return default_value;
	}


	const string XentuConfig::GetSetting(const string group, const string sub_group, const string name, const string default_value)
	{
		try {
			auto m_group = m_data.at(group);
			auto m_subgroup = m_group.at(sub_group);
			return m_subgroup.at(name);
		}
		catch (std::exception e)
		{
			XEN_LOG("- Config warning: GetSetting [%s.%s.%s] missing.\n",  group.c_str(), sub_group.c_str(), name.c_str());
		}
		return default_value;
	}
	const bool XentuConfig::GetSettingBool(const string group, const string sub_group, const string name, const bool default_value)
	{
		try {
			auto m_group = m_data.at(group);
			auto m_subgroup = m_group.at(sub_group);
			return m_subgroup.at(name);
		}
		catch (std::exception e)
		{
			XEN_LOG("- Config warning: GetSettingBool [%s.%s.%s] missing.\n",  group.c_str(), sub_group.c_str(), name.c_str());
		}
		return default_value;
	}
	const int XentuConfig::GetSettingInt(const string group, const string sub_group, const string name, const int default_value)
	{
		try {
			auto m_group = this->m_data.at(group);
			auto m_subgroup = m_group.at(sub_group);
			return m_subgroup.at(name);
		}
		catch (std::exception e)
		{
			XEN_LOG("- Config warning: GetSettingInt [%s.%s.%s] missing.\n",  group.c_str(), sub_group.c_str(), name.c_str());
		}
		return default_value;
	}

	
	XentuConfig::~XentuConfig()
	{
		XEN_LOG("- Destroyed XentuConfig.\n");
	}
}

#endif