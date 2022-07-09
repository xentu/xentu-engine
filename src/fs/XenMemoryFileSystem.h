#ifndef XEN_MEMORY_FILE_SYSTEM_H
#define XEN_MEMORY_FILE_SYSTEM_H

#include "XenFileSystem.h"

namespace xen
{
	CLASS_PTR(XenMemoryFile);
    
	class XenMemoryFileSystem final : public XenFileSystem
	{
	public:
		XenMemoryFileSystem();
		~XenMemoryFileSystem();
    
		/* Initialize filesystem with a base path */
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
    
	private:
		bool m_IsInitialized;
		TFileList m_FileList;
	};
    
}

#endif