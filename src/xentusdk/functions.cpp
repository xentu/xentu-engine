#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <Shldisp.h>
#include <direct.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

extern "C" {
#include "libraries/miniz.h"
}

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

LPCTSTR get_perm_env_variable(LPCTSTR value)
{
    LPCTSTR result = "";
    HKEY hKey;
    LPCTSTR keyPath = TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
    LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_QUERY_VALUE, &hKey);
    if (lOpenStatus == ERROR_SUCCESS) 
    {
        DWORD dwType = REG_SZ;
        unsigned char valueBuffer[4096];
        DWORD value_length = sizeof(valueBuffer);
        //RegQueryValueEx(hKey, value, NULL, &dwType, (LPBYTE)valueBuffer, &value_length);
        LSTATUS lQueryStatus = RegQueryValueEx(hKey, value, NULL, &dwType, (unsigned char*)valueBuffer, &value_length);
        if (lQueryStatus == ERROR_SUCCESS) {
            result = LPCTSTR(valueBuffer);
        }
        RegCloseKey(hKey);
    }
    else {
        std::cout << "failed to open key" << std::endl;
    }

    return result;
}

bool set_perm_env_variable(LPCTSTR value, LPCTSTR data)
{
    HKEY hKey;
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

/// <summary>
/// Call this to extract the contents of a zip file into a directory.
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
int extract_zip_archive(const char* filename, const char* output_directory, const bool overwrite) {
	mz_zip_archive zip_archive;
	memset(&zip_archive, 0, sizeof(zip_archive));
	mz_bool status = mz_zip_reader_init_file(&zip_archive, filename, 0);
	if (!status) {
		printf("mz_zip_reader_init_file() failed!\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++) {
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
			printf("mz_zip_reader_file_stat() failed!\n");
			mz_zip_reader_end(&zip_archive);
			return EXIT_FAILURE;
		}

		auto out_path = std::string(output_directory) + PATH_SEPARATOR + std::string(file_stat.m_filename);

		if (mz_zip_reader_is_file_a_directory(&zip_archive, i) == MZ_TRUE) {
			// it's a directory, so attempt to create it.
			auto ret = _mkdir(out_path.c_str());
			//printf("- created directory: \"%s\"\n", file_stat.m_filename);
		}
		else {
			// it's a file, extract it.
			auto fres = mz_zip_reader_extract_file_to_file(&zip_archive, file_stat.m_filename, out_path.c_str(), 0);
            //std::cout << "- success?: " << fres << std::endl;
			//printf("- extracted: \"%s\" to %s\n", file_stat.m_filename, out_path.c_str());
		}
	}

	mz_zip_reader_end(&zip_archive);

	return 0;
}

#endif


bool file_exists(const std::string& file) {
	struct stat buffer;   
	return (stat (file.c_str(), &buffer) == 0); 
}



int unzip_template(const std::string zipFile, const std::string dst_folder)
{
    #if defined(WIN32) || defined(_WIN32)
    int res = extract_zip_archive(zipFile.c_str(), dst_folder.c_str(), false);
    std::cout << "Extract Code: " << res << std::endl;
    #endif
    
    return 0;
}

