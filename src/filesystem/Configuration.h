#ifndef XEN_CONFIGURATION_HPP
#define XEN_CONFIGURATION_HPP

#include <string>

namespace xen
{
	class Configuration
	{
	public:
		Configuration();
		static Configuration* parse_file(std::string filename);


		std::string m_game_title;
		unsigned int m_screen_width;
		unsigned int m_screen_height;
		unsigned int m_update_frequency;
		unsigned int m_viewport_width;
		unsigned int m_viewport_height;
		bool m_vsync;
	};
}

#endif