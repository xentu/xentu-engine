#pragma once

#include <string>
#include <vector>

namespace xen
{
	class StringUtils final
	{
	public:
		static void Split(std::vector<std::string>& tokens, const std::string& text, char delimeter);
		static std::string Replace(std::string string, const std::string& search, const std::string& replace);
		static bool EndsWith(const std::string& fullString, const std::string& ending);
		static bool StartsWith(const std::string& fullString, const std::string& starting);
	};
}