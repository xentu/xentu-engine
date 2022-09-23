#include "XenMemoryFileSystem.h"
#include "XenMemoryFile.h"
#include "XenStringUtils.h"

namespace xen
{
	XenMemoryFileSystem::XenMemoryFileSystem()
	: m_IsInitialized(false)
	{ }


	XenMemoryFileSystem::~XenMemoryFileSystem()
	{ }


	void XenMemoryFileSystem::Initialize()
	{
		if (m_IsInitialized) {
			return;
		}
		m_IsInitialized = true;
	}


	void XenMemoryFileSystem::Shutdown()
	{
		m_FileList.clear();
		m_IsInitialized = false;
	}


	bool XenMemoryFileSystem::IsInitialized() const
	{
		return m_IsInitialized;
	}


	const std::string& XenMemoryFileSystem::BasePath() const
	{
		static std::string basePath = "/";
		return basePath;
	}


	const XenFileSystem::TFileList& XenMemoryFileSystem::FileList() const
	{
		return m_FileList;
	}


	bool XenMemoryFileSystem::IsReadOnly() const
	{
		return false;
	}


	XenFilePtr XenMemoryFileSystem::DoOpenFile(const XenFileInfo& filePath, int mode)
	{
		XenFileInfo fileInfo(BasePath(), filePath.AbsolutePath(), false);
		XenFilePtr file = FindFile(fileInfo);
		bool isExists = (file != nullptr);
		if (!isExists)
		{
			file.reset(new XenMemoryFile(fileInfo));
		}
		file->Open(mode);
		
		if (!isExists && file->IsOpened())
		{
			m_FileList.insert(file);
		}
		
		return file;
	}


	void XenMemoryFileSystem::DoCloseFile(XenFilePtr file)
	{
		if (file) {
			file->Close();
		}
	}


	bool XenMemoryFileSystem::DoCreateFile(const XenFileInfo& filePath)
	{
		bool result = false;
		if (!IsReadOnly() && !IsFileExists(filePath)) {
			XenFileInfo fileInfo(BasePath(), filePath.AbsolutePath(), false);
			XenFilePtr file = DoOpenFile(filePath, XenFile::Out | XenFile::Truncate);
        	if (file) {
				result = true;
				file->Close();
			}
		}
		else {
			result = true;
		}
		return result;
	}


	bool XenMemoryFileSystem::DoRemoveFile(const XenFileInfo& filePath) {
		bool result = true;

		XenFilePtr file = FindFile(filePath);
		if (!IsReadOnly() && file) {
			XenFileInfo fileInfo(BasePath(), file->FileInfo().AbsolutePath(), false);
			m_FileList.erase(file);
		}
		return result;
	}


	bool XenMemoryFileSystem::DoCopyFile(const XenFileInfo& src, const XenFileInfo& dest)
	{
		bool result = false;
		if (!IsReadOnly()) {
			XenMemoryFilePtr srcFile = std::static_pointer_cast<XenMemoryFile>(FindFile(src));
			XenMemoryFilePtr dstFile = std::static_pointer_cast<XenMemoryFile>(DoOpenFile(dest, XenFile::Out));
			if (srcFile && dstFile) {
				dstFile->m_Data.assign(srcFile->m_Data.begin(), srcFile->m_Data.end());
				dstFile->Close();
				result = true;
			}
		}
		return result;
	}


	bool XenMemoryFileSystem::DoRenameFile(const XenFileInfo& src, const XenFileInfo& dest)
	{
		bool result = DoCopyFile(src, dest);
		if (result) {
			result = DoRemoveFile(src);
		}
		return result;
	}


	bool XenMemoryFileSystem::IsFileExists(const XenFileInfo& filePath) const
	{
		return (FindFile(BasePath() + filePath.AbsolutePath()) != nullptr);
	}


	bool XenMemoryFileSystem::IsFile(const XenFileInfo& filePath) const
	{
		XenFilePtr file = FindFile(filePath);
		if (file) {
			return !file->FileInfo().IsDir();
		}
		return false;
	}


	bool XenMemoryFileSystem::IsDir(const XenFileInfo& dirPath) const
	{
		XenFilePtr file = FindFile(dirPath);
		if (file) {
			return file->FileInfo().IsDir();
		}
		return false;
	}


	XenFilePtr XenMemoryFileSystem::FindFile(const XenFileInfo& fileInfo) const
	{
		TFileList::const_iterator it = std::find_if(m_FileList.begin(), m_FileList.end(), [&](XenFilePtr file) {
			return file->FileInfo() == fileInfo;
		});

		if (it != m_FileList.end()) {
			return *it;
		}
    
		return nullptr;
	}
}