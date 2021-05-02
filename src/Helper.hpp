#pragma once

#include <fstream>
#include <sstream>

namespace xen
{
	class Helper
	{
	public:
		static std::string read_text_file(const std::string& filePath) {
			// alternative method http://www.lua.org/pil/24.1.html
			std::ifstream inFile{ filePath };
			std::string file_contents{ std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>() };
			return file_contents;
		}
	};
}