#ifndef XEN_VIRTUAL_FILE_SYSTEM_H
#define XEN_VIRTUAL_FILE_SYSTEM_H

#include "XenFileSystem.h"
#include "XenFile.h"

namespace xen
{
	CLASS_PTR(XenFile)
	CLASS_PTR(XenFileSystem)
	CLASS_PTR(XenVirtualFileSystem)
    
	extern void vfs_default();
	extern void vfs_initialize();
	extern void vfs_shutdown();
	extern XenVirtualFileSystemPtr vfs_get_global();

	struct VfsBufferResult {uint8_t* buffer; uint64_t length; uint64_t r_length;};

	class XenVirtualFileSystem final
	{
	public:
		typedef std::list<XenFileSystemPtr> TFileSystemList;
		typedef std::unordered_map<std::string, XenFileSystemPtr> TFileSystemMap;
    
		struct SSortedAlias
		{
			std::string alias;
			XenFileSystemPtr filesystem;
        
			SSortedAlias(const std::string& a,
                      XenFileSystemPtr fs)
			: alias(a)
			, filesystem(fs)
			{ }
		};
		typedef std::list<SSortedAlias> TSortedAliasList;
    
	public:
		XenVirtualFileSystem();
    	~XenVirtualFileSystem();
    	
		/*
		 * Register new filesystem. Alias is base prefix to file access.
		 * For ex. registered filesystem has base path '/home/media', but registered
		 * with alias '/', so it possible to access files with path '/filename'
		 * instead of '/home/media/filename
		 */
		void AddFileSystem(const std::string& alias, XenFileSystemPtr filesystem);

		/*
		 * Remove registered filesystem
		 */
		void RemoveFileSystem(const std::string& alias);
    
		/*
		 * Check if filesystem with 'alias' registered
		 */
		bool IsFileSystemExists(const std::string& alias) const;
    
		/*
		 * Get filesystem with 'alias'
		 */
		XenFileSystemPtr GetFilesystem(const std::string& alias);

		/*
		 * Iterate over all registered filesystems and find first occurrences of file
		 */
		XenFilePtr OpenFile(const XenFileInfo& filePath, XenFile::FileMode mode);

		/*
		 * Close opened file if it was opened via OpenFirstFile(..)
		 */
		void CloseFile(XenFilePtr file);

		/**
		 * Read the entire contents of a text file and return it as a string.
		 */
		std::string XenVirtualFileSystem::ReadAllText(const std::string filename);

		/**
		 * Read the entire contents of a file and return it as a tuple of array,length
		 */
		VfsBufferResult ReadAllData(const std::string filename);
    
	private:
		TFileSystemMap m_FileSystem;
		TSortedAliasList m_SortedAlias;
		std::unordered_map<uintptr_t, XenFileSystemPtr> m_OpenedFiles;
	};
    
};

#endif