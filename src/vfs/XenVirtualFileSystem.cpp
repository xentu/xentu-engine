#include "XenVirtualFileSystem.h"
#include "XenNativeFileSystem.h"
#include "XenZipFileSystem.h"
#include "../StringUtils.h"
#include "../Exceptions.h"
#include "../Globals.h"

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


	void vfs_default()
	{
		// get the global vfs isntance.
		XenVirtualFileSystemPtr vfs = vfs_get_global();
		
		// create a pointing to a potential game.dat file.
		if (XEN_FILE_EXISTS("./game.dat")) {
			XenFileSystemPtr game_fs(new XenZipFileSystem("./game.dat", "/"));
			game_fs->Initialize();
			if (game_fs->IsInitialized()) {
				vfs->AddFileSystem("/", game_fs);
				return;
			}
		}

		// create a pointing to a potential game.zip file.
		if (XEN_FILE_EXISTS("./game.zip")) {
			XenFileSystemPtr gameZip_fs(new XenZipFileSystem("./game.zip", "/"));
			gameZip_fs->Initialize();
			if (gameZip_fs->IsInitialized()) {
				vfs->AddFileSystem("/", gameZip_fs);
				return;
			}
		}
		
		// we made it this far without a default mount, so use the regular file system instead.
		XenFileSystemPtr root_fs(new XenNativeFileSystem("./"));
		root_fs->Initialize();
		vfs->AddFileSystem("/", root_fs);
	}


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
		if (!StringUtils::EndsWith(a, "/")) {
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
		if (!StringUtils::EndsWith(a, "/")) {
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
    	(void)std::all_of(m_SortedAlias.begin(), m_SortedAlias.end(), [&](const TSortedAliasList::value_type& fs) {
			const std::string& alias = fs.alias;
			XenFileSystemPtr filesystem = fs.filesystem;
			if (StringUtils::StartsWith(filePath.BasePath(), alias) && filePath.AbsolutePath().length() != alias.length()) {
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


	std::string XenVirtualFileSystem::ReadAllText(const std::string filename)
	{
		xen::XenFilePtr file = this->OpenFile(xen::XenFileInfo(filename), xen::XenFile::In);
		if (file && file->IsOpened())
		{
			uint64_t length = file->Size(); // not always accurate.

			size_t data_len = 4096 * 10;
			if (length > data_len) 
			{
				// big enough that we need to allocate it onto the heap.
				uint8_t* data2 = new uint8_t[length];
				uint64_t r_length = file->Read(data2, length);
				auto result2 = std::string(data2, data2 + r_length);
				this->CloseFile(file);
				return result2;
			}
			else
			{
				uint8_t data[4096 * 10]; // create a buffer.
				uint64_t r_length = file->Read(data, length); // returns true length.
				auto result = std::string(data, data + r_length);
				this->CloseFile(file);
				return result;
			}
		}

		throw XentuNotFoundException(filename.c_str());
		return "";
	}


	VfsBufferResult XenVirtualFileSystem::ReadAllData(const std::string filename)
	{
		xen::XenFilePtr file = this->OpenFile(xen::XenFileInfo(filename), xen::XenFile::In);
		if (file && file->IsOpened())
		{
			uint64_t length = file->Size(); // not always accurate.
			size_t data_len = 4096 * 10;
			if (length > data_len) 
			{
				// big enough that we need to allocate it onto the heap.
				uint8_t* data2 = new uint8_t[length];
				uint64_t r_length = file->Read(data2, length);
				this->CloseFile(file);
				return VfsBufferResult { data2, length, r_length, true };
			}
			else
			{
				uint8_t data[4096 * 10]; // create a buffer.
				uint64_t r_length = file->Read(data, length); // returns true length.
				this->CloseFile(file);
				return VfsBufferResult { data , length, r_length, false };
			}
			
		}
		uint8_t r_data[1];
		return VfsBufferResult { r_data, 0, 0, false };
	}
}