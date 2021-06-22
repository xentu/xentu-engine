#pragma once

#include <sys/stat.h>
#include <Windows.h>
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

		static bool file_exists(const std::string& filePath) {
			struct stat buffer;   
			return (stat (filePath.c_str(), &buffer) == 0); 
		}

		static std::string get_current_directory()
		{
			// TODO: Currently requires Windows.h, try to find an alternative.
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
		}

		static std::string get_console_path()
		{
			char path[256];
			#ifdef _WIN32
				_getcwd(path, 256);
			#else
				getcwd(path, 256);
			#endif
			return path;
		}
	};
}