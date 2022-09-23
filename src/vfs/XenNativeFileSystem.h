#pragma once

#include "XenFileSystem.h"

namespace xen
{
	struct SDir;
	
	CLASS_PTR(XenNativeFile)
    
	class XenNativeFileSystem final : public XenFileSystem
	{
	public:
		/* Constructor, create with a base path */
		XenNativeFileSystem(const std::string& basePath);
		~XenNativeFileSystem();

		/* Initialize filesystem */
		virtual void Initialize() override;
		
		/* Shutdown filesystem */
		virtual void Shutdown() override;
    
		/* Check if filesystem is initialized */
		virtual bool IsInitialized() const override;
		
		/* Get base path */
		virtual const std::string& BasePath() const override;
		
		/* Retrieve file list according filter */
		virtual const TFileList& FileList() const override;

		/* Check is readonly filesystem */
		virtual bool IsReadOnly() const override;
		
		/* Open existing file for reading, if not exists return null */
		virtual XenFilePtr DoOpenFile(const XenFileInfo& filePath, int mode) override;

		/* Close file */
		virtual void DoCloseFile(XenFilePtr file) override;
    
	 	/* Create file on writeable filesystem. Return true if file already exists */
		virtual bool DoCreateFile(const XenFileInfo& filePath) override;
		
		/* Remove existing file on writable filesystem */
		virtual bool DoRemoveFile(const XenFileInfo& filePath) override;

		/* Copy existing file on writable filesystem */
		virtual bool DoCopyFile(const XenFileInfo& src, const XenFileInfo& dest) override;
		
		/* Rename existing file on writable filesystem */
		virtual bool DoRenameFile(const XenFileInfo& src, const XenFileInfo& dest) override;
	
		/* Check if file exists on filesystem */
		virtual bool IsFileExists(const XenFileInfo& filePath) const override;
		
		/* Check is file */
		virtual bool IsFile(const XenFileInfo& filePath) const override;
    	
		/* Check is dir */
		virtual bool IsDir(const XenFileInfo& dirPath) const override;
    
	private:
		XenFilePtr FindFile(const XenFileInfo& fileInfo) const;
		void BuildFilelist(SDir* dir, std::string basePath, TFileList& outFileList);
    
	private:
		std::string m_BasePath;
		bool m_IsInitialized;
		TFileList m_FileList;
	}; 
}