#include <exception>

#include "Globals.h"
#include "Config.h"

using namespace std;

namespace xen
{
	Config::Config(const char* json)
	{
		m_data = nlohmann::json::parse(json);
		try {
			auto m_game = m_data.at("game");

			title = m_game.at("title");
			title = this->GetSetting("game", "title", "");
			entry_point = this->GetSetting("game", "entry_point", "");
			v_sync = this->GetSettingBool("game", "v_sync", true);
			fullscreen = this->GetSettingBool("game", "fullscreen", false);
			resizable = this->GetSettingBool("game", "resizable", true);
			update_frequency = this->GetSettingInt("game", "update_frequency", 60);
			draw_frequency = this->GetSettingInt("game", "draw_frequency", 60);

			int win_x = this->GetSettingInt("game", "window", "x", -1);
			int win_y = this->GetSettingInt("game", "window", "y", -1);
			int win_w = this->GetSettingInt("game", "window", "width", 640);
			int win_h = this->GetSettingInt("game", "window", "height", 480);
			window = { win_x, win_y, win_w, win_h };

			int vp_w = this->GetSettingInt("game", "viewport", "width", 640);
			int vp_h = this->GetSettingInt("game", "viewport", "height", 480);
			int vp_m = this->GetSettingInt("game", "viewport", "mode", 0);
			viewport = { vp_w, vp_h, vp_m };

			string ren_engine_s = this->GetSetting("game", "renderer", "engine", "sdl2");
			RenderEngine ren_engine = RenderEngine::sdl2;
			if (ren_engine_s == "xna") ren_engine = RenderEngine::xna;
			if (ren_engine_s == "open_tk") ren_engine = RenderEngine::open_tk;
			string ren_teq_s = this->GetSetting("game", "renderer", "technique", "immediate");
			RenderTechnique ren_teq = RenderTechnique::immediate;
			if (ren_engine_s == "deferred") ren_teq = RenderTechnique::deferred;
			bool ren_acc = this->GetSettingBool("game", "renderer", "accelerated", true);
			renderer = { ren_engine, ren_teq, ren_acc };

			int a_freq = this->GetSettingInt("game", "audio", "frequency", 44100);
			int a_channels = this->GetSettingInt("game", "audio", "channels", 2);
			audio = { a_freq, a_channels };

			XEN_ECHO("> Entry point: %s\n", entry_point.c_str());
		}
		catch (std::exception e)
		{
			XEN_ERROR("Config error! %s\n", e.what());
			//
		}
	}

	const string Config::GetSetting(const string group, const string name, const string default_value)
	{
		try {
			auto m_group = m_data.at(group);
			return m_group.at(name);
		}
		catch (std::exception e)
		{
			XEN_WARN("> Config warning: GetSetting [%s.%s] missing.\n", group.c_str(), name.c_str());
		}
		return default_value;
	}

	const bool Config::GetSettingBool(const string group, const string name, const bool default_value)
	{
		try {
			auto m_group = m_data.at(group);
			return m_group.at(name);
		}
		catch (std::exception e)
		{
			XEN_WARN("> Config warning: SetSetting [%s.%s] missing.\n", group.c_str(), name.c_str());
		}
		return default_value;
	}
	
	const int Config::GetSettingInt(const string group, const string name, const int default_value)
	{
		try {
			auto m_group = m_data.at(group);
			return m_group.at(name);
		}
		catch (std::exception e)
		{
			XEN_WARN("> Config warning: GetSettingInt [%s.%s] missing.\n", group.c_str(), name.c_str());
		}
		return default_value;
	}

	const string Config::GetSetting(const string group, const string sub_group, const string name, const string default_value)
	{
		try {
			auto m_group = m_data.at(group);
			auto m_subgroup = m_group.at(sub_group);
			return m_subgroup.at(name);
		}
		catch (std::exception e)
		{
			XEN_WARN("> Config warning: GetSetting [%s.%s.%s] missing.\n",  group.c_str(), sub_group.c_str(), name.c_str());
		}
		return default_value;
	}
	const bool Config::GetSettingBool(const string group, const string sub_group, const string name, const bool default_value)
	{
		try {
			auto m_group = m_data.at(group);
			auto m_subgroup = m_group.at(sub_group);
			return m_subgroup.at(name);
		}
		catch (std::exception e)
		{
			XEN_WARN("> Config warning: GetSettingBool [%s.%s.%s] missing.\n",  group.c_str(), sub_group.c_str(), name.c_str());
		}
		return default_value;
	}
	const int Config::GetSettingInt(const string group, const string sub_group, const string name, const int default_value)
	{
		try {
			auto m_group = this->m_data.at(group);
			auto m_subgroup = m_group.at(sub_group);
			return m_subgroup.at(name);
		}
		catch (std::exception e)
		{
			XEN_WARN("> Config warning: GetSettingInt [%s.%s.%s] missing.\n",  group.c_str(), sub_group.c_str(), name.c_str());
		}
		return default_value;
	}
	
	Config::~Config()
	{ }
}