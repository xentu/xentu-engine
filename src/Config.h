#pragma once

#include <string>
#include <json/json.hpp>

using namespace std;

namespace xen
{
	enum class RenderEngine { open_gl, direct_x, vulkan };

	struct ConfigWindow
	{
		int x, y, width, height; 
	};


	struct ConfigViewport
	{
		int width, height, mode;
	};


	struct ConfigRenderer
	{
		int mode;
		RenderEngine engine;
		bool accelerated;
	};


	struct ConfigAudio
	{
		int frequency;
		int channels;
	};


	class Config
	{
		public:
			Config(const char* filename);
			~Config(void);

			const string GetSetting(const string group, const string name, const string default_value);
			const bool GetSettingBool(const string group, const string name, const bool default_value);
			const int GetSettingInt(const string group, const string name, const int default_value);

			const string GetSetting(const string group, const string sub_group, const string name, const string default_value);
			const bool GetSettingBool(const string group, const string sub_group, const string name, const bool default_value);
			const int GetSettingInt(const string group, const string sub_group, const string name, const int default_value);

		public:
			std::string title;
			std::string entry_point;
			std::string icon;
			bool v_sync;
			bool fullscreen;
			bool resizable;
			int update_frequency;
			int draw_frequency;
			ConfigWindow window;
			ConfigRenderer renderer;
			ConfigViewport viewport;
			ConfigAudio audio;

		private:
			nlohmann::json m_data;
	};
}