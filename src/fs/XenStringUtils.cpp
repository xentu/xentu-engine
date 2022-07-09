#include "XenStringUtils.h"

namespace xen
{
	void XenStringUtils::Split(std::vector<std::string>& tokens, const std::string& text, char delimeter)
	{
		size_t start = 0;
		size_t end = 0;
		while ((end = text.find(delimeter, start)) != std::string::npos) {
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(text.substr(start));
	}


	std::string XenStringUtils::Replace(std::string string, const std::string& from, const std::string& to)
	{
		size_t pos = 0;
		while ((pos = string.find(from, pos)) != std::string::npos) {
			string.replace(pos, from.length(), to);
			pos += to.length();
		}
		return string;
	}


	bool XenStringUtils::EndsWith(std::string const& fullString, std::string const& ending)
	{
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		return false;
	}


	bool XenStringUtils::StartsWith(std::string const& fullString, std::string const& starting)
	{
		if (fullString.length() >= starting.length()) {
			return (0 == fullString.compare(0, starting.length(), starting));
		}
		return false;
	}
}