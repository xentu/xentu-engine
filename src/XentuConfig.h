#pragma once

#include <string>
#include <json/json.hpp>

using namespace std;

namespace xen
{
	enum class XentuRenderEngine { sdl2, xna, open_tk };
	enum class XentuRenderTechnique { immediate, deferred };


	struct XentuConfigWindow
	{
		int x, y, width, height; 
	};


	struct XentuConfigRenderer
	{
		XentuRenderEngine engine;
		XentuRenderTechnique technique;
		bool accelerated;
	};


	class XentuConfig
	{
		public:
			XentuConfig(const char* filename);
			~XentuConfig(void);

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
			XentuConfigWindow window;
			XentuConfigRenderer renderer;

		private:
			nlohmann::json m_data;
	};
}