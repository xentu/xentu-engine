#pragma once

#include <string>
#include <json/json.hpp>

using namespace std;

namespace xen
{
	enum class RenderEngine { sdl2, xna, open_tk };
	enum class RenderTechnique { immediate, deferred };


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
		RenderEngine engine;
		RenderTechnique technique;
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
			std::string language;
			std::string entry_point;
			bool v_sync;
			bool fullscreen;
			int update_frequency;
			ConfigWindow window;
			ConfigRenderer renderer;
			ConfigViewport viewport;
			ConfigAudio audio;

		private:
			nlohmann::json m_data;
	};
}