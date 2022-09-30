#include "XenNativeFileSystem.h"
#include "XenNativeFile.h"
#include "XenStringUtils.h"

namespace xen
{
	const uint64_t kChunkSize = 1024;


	XenNativeFileSystem::XenNativeFileSystem(const std::string& basePath)
	: m_BasePath(basePath)
	, m_IsInitialized(false)
	{
		if (!XenStringUtils::EndsWith(m_BasePath, "/"))
		{
			m_BasePath += "/";
		}
	}


	XenNativeFileSystem::~XenNativeFileSystem()
	{ }


	void XenNativeFileSystem::Initialize()
	{
		if (m_IsInitialized) {
			return;
		}
    
		DIR *dir = static_cast<DIR*>(opendir(BasePath().c_str()));
		if (dir) {
			BuildFilelist(dir, BasePath(), m_FileList);
			m_IsInitialized = true;
			closedir(dir);
		}
	}


	void XenNativeFileSystem::Shutdown()
	{
		m_BasePath = "";
		m_FileList.clear();
		m_IsInitialized = false;
	}


	bool XenNativeFileSystem::IsInitialized() const
	{
		return m_IsInitialized;
	}

	
	const std::string& XenNativeFileSystem::BasePath() const
	{
		return m_BasePath;
	}


	const XenFileSystem::TFileList& XenNativeFileSystem::FileList() const
	{
		return m_FileList;
	}


	bool XenNativeFileSystem::IsReadOnly() const
	{
		if (!IsInitialized()) {
			return true;
		}
    
		struct stat fileStat;
		if (stat(BasePath().c_str(), &fileStat) < 0) {
			return false;
		}
		return (fileStat.st_mode & S_IWUSR);
	}


	XenFilePtr XenNativeFileSystem::DoOpenFile(const XenFileInfo& filePath, int mode)
	{
		XenFileInfo fileInfo(BasePath(), filePath.AbsolutePath(), false);
		XenFilePtr file = FindFile(fileInfo);
		bool isExists = (file != nullptr);
		if (!isExists) {
			mode |= XenFile::Truncate;
			file.reset(new XenNativeFile(fileInfo));
		}
		file->Open(mode);
		
		if (!isExists && file->IsOpened()) {
			m_FileList.insert(file);
		}
		
		return file;
	}


	void XenNativeFileSystem::DoCloseFile(XenFilePtr file)
	{
		if (file) {
			file->Close();
		}
	}


	bool XenNativeFileSystem::DoCreateFile(const XenFileInfo& filePath)
	{
		bool result = false;
		if (!IsReadOnly() && !IsFileExists(filePath))
		{
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


	bool XenNativeFileSystem::DoRemoveFile(const XenFileInfo& filePath)
	{
		bool result = true;
		XenFilePtr file = FindFile(filePath);
		if (!IsReadOnly() && file) {
			XenFileInfo fileInfo(BasePath(), file->FileInfo().AbsolutePath(), false);
			if (remove(fileInfo.AbsolutePath().c_str()))	{
				m_FileList.erase(file);
			}
		}
		return result;
	}


	bool XenNativeFileSystem::DoCopyFile(const XenFileInfo& src, const XenFileInfo& dest)
	{
		bool result = false;
		if (!IsReadOnly()) {
			XenFilePtr fromFile = FindFile(src);
			XenFilePtr toFile = DoOpenFile(dest, XenFile::Out);
			
			if (fromFile && toFile)	{
				uint64_t size = kChunkSize;
				std::vector<uint8_t> buff((size_t)size);
				do	{
					fromFile->Read(buff.data(), kChunkSize);
					toFile->Write(buff.data(), size);
				}
				while (size == kChunkSize);
					
				result = true;
			}
		}
		
		return result;
	}


	bool XenNativeFileSystem::DoRenameFile(const XenFileInfo& src, const XenFileInfo& dest)
	{
		if (!IsReadOnly()) {
			return false;
		}
		
		bool result = false;
		
		XenFilePtr fromFile = FindFile(src);
		XenFilePtr toFile = FindFile(dest);
		if (fromFile && !toFile) {
			XenFileInfo toInfo(BasePath(), dest.AbsolutePath(), false);
			if (rename(fromFile->FileInfo().AbsolutePath().c_str(), toInfo.AbsolutePath().c_str())) {
				m_FileList.erase(fromFile);
				toFile = DoOpenFile(dest, XenFile::In);
				if (toFile) {
					result = true;
					toFile->Close();
				}
			}
		}
		
		return result;
	}


	bool XenNativeFileSystem::IsFileExists(const XenFileInfo& filePath) const
	{
		return (FindFile(BasePath() + filePath.AbsolutePath()) != nullptr);
	}


	bool XenNativeFileSystem::IsFile(const XenFileInfo& filePath) const
	{
		XenFilePtr file = FindFile(filePath);
		if (file) {
			return !file->FileInfo().IsDir();
		}
		return false;
	}


	bool XenNativeFileSystem::IsDir(const XenFileInfo& dirPath) const
	{
		XenFilePtr file = FindFile(dirPath);
		if (file) {
			return file->FileInfo().IsDir();
		}
		return false;
	}


	XenFilePtr XenNativeFileSystem::FindFile(const XenFileInfo& fileInfo) const
	{
		TFileList::const_iterator it = std::find_if(m_FileList.begin(), m_FileList.end(), [&](XenFilePtr file) {
			return file->FileInfo() == fileInfo;
		});
		
		if (it != m_FileList.end()) {
			return *it;
		}
		
		return nullptr;
	}


	void XenNativeFileSystem::BuildFilelist(DIR* dir, std::string basePath, TFileList& outFileList)
	{
		if (!XenStringUtils::EndsWith(basePath, "/")) {
			basePath += "/";
		}
		
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL) {
			std::string filename = ent->d_name;
			std::string filepath = basePath + filename;
			DIR *childDir = static_cast<DIR*>(opendir(filepath.c_str()));
			bool isDotOrDotDot = XenStringUtils::EndsWith(filename, ".") && childDir;
			if (childDir && !isDotOrDotDot) {
					filename += "/";
			}
			
			XenFileInfo fileInfo(basePath, filename, childDir != NULL);
			if (!FindFile(fileInfo)) {
				XenFilePtr file(new XenNativeFile(fileInfo));
				outFileList.insert(file);
			}
			
			if (childDir) {
				if (!isDotOrDotDot) {
					BuildFilelist(childDir, (childDir ? filepath : basePath), outFileList);
				}
				closedir(childDir);
			}
		}
	}
}