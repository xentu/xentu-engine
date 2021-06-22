#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#endif

using namespace std;


/// <summary>
/// Get the users current path, usefull when a user is calling xentusdk from
/// another location on their computer. 
/// </summary>
string get_console_path()
{
    char path[256];
    #ifdef _WIN32
        _getcwd(path, 256);
    #else
        getcwd(path, 256);
    #endif
    return path;
}


/// <summary>
/// Get the path of the SDK (where xentusdk.exe is).
/// </summary>
string get_sdk_path()
{
    char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}


string replace_first(string s, const std::string& toReplace, const std::string& replaceWith)
{
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return s;
    return s.replace(pos, toReplace.length(), replaceWith);
}


void write_text_file(const std::string& file, const std::string& content)
{
    FILE *f = fopen(file.c_str(), "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fputs(content.c_str(), f);
    fclose(f);
}


bool copy_file(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}