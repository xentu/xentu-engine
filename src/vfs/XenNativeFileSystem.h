#pragma once

#ifdef _WIN32
#include <lib/dirent-win.h>
#else
#include <dirent.h>
#endif

#include <sys/stat.h>
//#include <fstream>

#include "XenFileSystem.h"

namespace xen
{	
	CLASS_PTR(XenNativeFile)
    
	class XenNativeFileSystem final : public XenFileSystem
	{
	public:
		/* Constructor, create with a base path */
		XenNativeFileSystem(const std::string& basePath);
		~XenNativeFileSystem();

		/* Initialize filesystem */
		void Initialize() override;
		
		/* Shutdown filesystem */
		void Shutdown() override;
    
		/* Check if filesystem is initialized */
		bool IsInitialized() const override;
		
		/* Get base path */
		const std::string& BasePath() const override;
		
		/* Retrieve file list according filter */
		const TFileList& FileList() const override;

		/* Check is readonly filesystem */
		bool IsReadOnly() const override;
		
		/* Open existing file for reading, if not exists return null */
		XenFilePtr DoOpenFile(const XenFileInfo& filePath, int mode) override;

		/* Close file */
		void DoCloseFile(XenFilePtr file) override;
    
	 	/* Create file on writeable filesystem. Return true if file already exists */
		bool DoCreateFile(const XenFileInfo& filePath) override;
		
		/* Remove existing file on writable filesystem */
		bool DoRemoveFile(const XenFileInfo& filePath) override;

		/* Copy existing file on writable filesystem */
		bool DoCopyFile(const XenFileInfo& src, const XenFileInfo& dest) override;
		
		/* Rename existing file on writable filesystem */
		bool DoRenameFile(const XenFileInfo& src, const XenFileInfo& dest) override;
	
		/* Check if file exists on filesystem */
		bool IsFileExists(const XenFileInfo& filePath) const override;
		
		/* Check is file */
		bool IsFile(const XenFileInfo& filePath) const override;
    	
		/* Check is dir */
		bool IsDir(const XenFileInfo& dirPath) const override;
    
	private:
		XenFilePtr FindFile(const XenFileInfo& fileInfo) const;
		void BuildFilelist(DIR* dir, std::string basePath, TFileList& outFileList);
    
	private:
		std::string m_BasePath;
		bool m_IsInitialized;
		TFileList m_FileList;
	}; 
}