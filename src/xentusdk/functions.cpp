#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#else
#include <limits.h>
#include <unistd.h>
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
#ifdef _WIN32
    char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
#else
    char arg1[20];
    char buffer[PATH_MAX + 1] = {0};
    sprintf( arg1, "/proc/%d/exe", getpid() );
    readlink( arg1, buffer, 1024 );
    string::size_type pos = string(buffer).find_last_of("\\/");
    return string(buffer).substr(0, pos);
#endif
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


#if defined(WIN32) || defined(_WIN32) 
bool set_perm_env_variable(LPCTSTR value, LPCTSTR data)
{
    HKEY hKey;
    //LPCTSTR keyPath = TEXT("System\\CurrentControlSet\\Control\\Session Manager\\Environment");
    LPCTSTR keyPath = TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
    LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
    if (lOpenStatus == ERROR_SUCCESS) 
    {
        LSTATUS lSetStatus = RegSetValueEx(hKey, value, 0, REG_SZ,(LPBYTE)data, strlen(data) + 1);
        RegCloseKey(hKey);

        if (lSetStatus == ERROR_SUCCESS)
        {
            SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
            return true;
        }
        else
        {
            std::cout << "Failed to set environment path. " << lSetStatus << std::endl;
            return false;
        }
    }

    std::cout << "Failed to open environment key." << std::endl;
    return false;
}
#endif