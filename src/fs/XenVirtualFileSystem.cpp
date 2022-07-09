#include "XenVirtualFileSystem.h"
#include "XenStringUtils.h"

namespace xen
{
	static XenVirtualFileSystemPtr g_FS;

	struct SAliasComparator
	{
		bool operator()(const XenVirtualFileSystem::SSortedAlias& a1, const XenVirtualFileSystem::SSortedAlias& a2) const
		{
			return a1.alias.length() > a2.alias.length();
		}
	};


	void vfs_initialize()
	{
		if (!g_FS)
		{
			g_FS.reset(new XenVirtualFileSystem());
		}
	}


	void vfs_shutdown()
	{
		g_FS = nullptr;
	}


	XenVirtualFileSystemPtr vfs_get_global()
	{
		return g_FS;
	}


	XenVirtualFileSystem::XenVirtualFileSystem()
	{ }

	
	XenVirtualFileSystem::~XenVirtualFileSystem()
	{
		std::for_each(m_FileSystem.begin(), m_FileSystem.end(), [](const TFileSystemMap::value_type& fs)
		{
			fs.second->Shutdown();
		});
	}


	void XenVirtualFileSystem::AddFileSystem(const std::string& alias, XenFileSystemPtr filesystem)
	{
		if (!filesystem) {
			return;
		}
    
		std::string a = alias;
		if (!XenStringUtils::EndsWith(a, "/")) {
			a += "/";
		}
    
		TFileSystemMap::const_iterator it = m_FileSystem.find(a);
		if (it == m_FileSystem.end()) {
			m_FileSystem[a] = filesystem;
			m_SortedAlias.push_back(SSortedAlias(a, filesystem));
			m_SortedAlias.sort(SAliasComparator());
		}
	}


	void XenVirtualFileSystem::RemoveFileSystem(const std::string& alias) {
		std::string a = alias;
		if (!XenStringUtils::EndsWith(a, "/")) {
			a += "/";
		}
    
		TFileSystemMap::const_iterator it = m_FileSystem.find(a);
		if (it == m_FileSystem.end()) {
			m_FileSystem.erase(it);
			// TODO: remove from alias list
		}
	}


	bool XenVirtualFileSystem::IsFileSystemExists(const std::string& alias) const
	{
		return (m_FileSystem.find(alias) != m_FileSystem.end());
	}


	XenFilePtr XenVirtualFileSystem::OpenFile(const XenFileInfo& filePath, XenFile::FileMode mode)
	{
		XenFilePtr file = nullptr;
    	std::all_of(m_SortedAlias.begin(), m_SortedAlias.end(), [&](const TSortedAliasList::value_type& fs) {
			const std::string& alias = fs.alias;
			XenFileSystemPtr filesystem = fs.filesystem;
			if (XenStringUtils::StartsWith(filePath.BasePath(), alias) && filePath.AbsolutePath().length() != alias.length()) {
				file = filesystem->DoOpenFile(filePath, mode);
			}
        
			if (file) {
				uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(file.get()));
				m_OpenedFiles[addr] = filesystem;
				return false;
			}
        
			return true;
		});
		return file;
	}


	void XenVirtualFileSystem::CloseFile(XenFilePtr file)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(file.get()));
		std::unordered_map<uintptr_t, XenFileSystemPtr>::const_iterator it = m_OpenedFiles.find(addr);
		if (it != m_OpenedFiles.end()) {
			it->second->DoCloseFile(file);
			m_OpenedFiles.erase(it);
		}
	}
}