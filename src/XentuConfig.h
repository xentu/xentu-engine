#ifndef XEN_CONFIG
#define XEN_CONFIG

#include <string>
#include <exception>
#include <json/json.hpp>

using namespace std;

namespace xen
{
	class XentuConfig
	{
		public:
			XentuConfig(const char* filename);
			~XentuConfig(void);
			const string XentuConfig::GetSetting(const string group, const string name, const string default_value);

		public:
			std::string m_language;

		private:
			nlohmann::json m_data;
	};
}

#endif