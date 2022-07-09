#ifndef XEN_FILE_SYSTEM_H
#define XEN_FILE_SYSTEM_H

#include "XenFile.h"


namespace xen
{
	CLASS_PTR(XenFile)

	class XenFileSystem
	{
		public:
			typedef std::set<XenFilePtr> TFileList;

		public:
			XenFileSystem() = default;
			~XenFileSystem() = default;
			
			/* Initialize filesystem, call this method as soon as possible */
			virtual void Initialize() = 0;
			
			/* Shutdown filesystem */
			virtual void Shutdown() = 0;
			
			/* Check if filesystem is initialized */
			virtual bool IsInitialized() const = 0;
			
			/* Get base path */
			virtual const std::string& BasePath() const = 0;
			
			/* Retrieve file list according filter */
			virtual const TFileList& FileList() const = 0;
			
			/* Check is readonly filesystem */
			virtual bool IsReadOnly() const = 0;
			
			/* Open existing file for reading, if not exists return null */
			virtual XenFilePtr DoOpenFile(const XenFileInfo& filePath, int mode) = 0;
			
			/* Close file */
			virtual void DoCloseFile(XenFilePtr file) = 0;
			
			/* Create file on writeable filesystem. Return true if file already exists */
			virtual bool DoCreateFile(const XenFileInfo& filePath) = 0;
			
			/* Remove existing file on writable filesystem */
			virtual bool DoRemoveFile(const XenFileInfo& filePath) = 0;
			
			/* Copy existing file on writable filesystem	*/
			virtual bool DoCopyFile(const XenFileInfo& src, const XenFileInfo& dest) = 0;
			
			/* Rename existing file on writable filesystem */
			virtual bool DoRenameFile(const XenFileInfo& src, const XenFileInfo& dest) = 0;
			
			/* Check if file exists on filesystem */
			virtual bool IsFileExists(const XenFileInfo& filePath) const = 0;
			
			/* Check is file */
			virtual bool IsFile(const XenFileInfo& filePath) const = 0;
			
			/* Check is dir */
			virtual bool IsDir(const XenFileInfo& dirPath) const = 0;
	};
}

#endif