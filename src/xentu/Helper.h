#pragma once

#include <sys/stat.h>

#if defined(WIN32) || defined(_WIN32) 
#include <Windows.h>
#include <direct.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#include <unistd.h>
#else
#include <limits.h>
#include <unistd.h>
#endif


#include <string>
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
			#ifdef _WIN32
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
			#elif __APPLE__
			char path[1024];
			uint32_t size = sizeof(path);
			if (_NSGetExecutablePath(path, &size) == 0)
    		{
				std::string::size_type pos = std::string(path).find_last_of("\\/");
				return std::string(path).substr(0, pos);
			}
			else
    			printf("buffer too small; need size %u\n", size);
				return "/";
			#else
			char arg1[20];
			char buffer[PATH_MAX + 1] = {0};
			sprintf( arg1, "/proc/%d/exe", getpid() );
			readlink( arg1, buffer, 1024 );
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
			#endif
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