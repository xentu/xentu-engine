#include "XenZipFileSystem.h"
#include <dirent.h>
#include <fstream>
#include "XenZipFile.h"
#include "XenStringUtils.h"

namespace xen
{
    std::unordered_map<std::string, XenZipPtr> XenZipFileSystem::s_OpenedZips;


    XenZipFileSystem::XenZipFileSystem(const std::string& zipPath, const std::string& basePath)
    : m_ZipPath(zipPath)
    , m_BasePath(basePath)
    , m_IsInitialized(false)
    {
    }


    XenZipFileSystem::~XenZipFileSystem()
    { }


    void XenZipFileSystem::Initialize()
    {
        if (m_IsInitialized)
        {
            return;
        }
        
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Zip = s_OpenedZips[m_ZipPath];
        if (!m_Zip) {
            m_Zip.reset(new XenZip(m_ZipPath));
            s_OpenedZips[m_ZipPath] = m_Zip;
        }
        m_IsInitialized = true;
    }


    void XenZipFileSystem::Shutdown()
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Zip = nullptr;
        if (s_OpenedZips[m_ZipPath].use_count() == 1) {
            s_OpenedZips.erase(m_ZipPath);
        }
        m_FileList.clear();
        m_IsInitialized = false;
    }


    bool XenZipFileSystem::IsInitialized() const
    {
        return m_IsInitialized;
    }


    const std::string& XenZipFileSystem::BasePath() const
    {
        return m_BasePath;
    }


    const XenFileSystem::TFileList& XenZipFileSystem::FileList() const
    {
        return m_FileList;
    }


    bool XenZipFileSystem::IsReadOnly() const
    {
        if (!IsInitialized()) {
            return true;
        }
        return m_Zip->IsReadOnly();
    }


    XenFilePtr XenZipFileSystem::DoOpenFile(const XenFileInfo& filePath, int mode)
    {
        XenFileInfo fileInfo(BasePath(), filePath.AbsolutePath(), false);
        XenFilePtr file = FindFile(fileInfo);
        bool isExists = (file != nullptr);
        if (!isExists) {        
            file.reset(new XenZipFile(fileInfo, m_Zip));
        }
        file->Open(mode);
        
        if (!isExists && file->IsOpened()) {
            m_FileList.insert(file);
        }
        return file;
    }


    void XenZipFileSystem::DoCloseFile(XenFilePtr file)
    {
        if (file) {
            file->Close();
        }
    }


    bool XenZipFileSystem::DoCreateFile(const XenFileInfo& filePath)
    {
        bool result = false;
        if (!IsFileExists(filePath)) {
            XenFilePtr file = DoOpenFile(filePath, XenFile::ReadWrite);
            if (file)
            {
                result = true;
                file->Close();
            }
        }
        else {
            result = true;
        }
        return result;
    }


    bool XenZipFileSystem::DoRemoveFile(const XenFileInfo& filePath)
    {
        return false; // TODO: Filesystem, temporally not suppoted
    }


    bool XenZipFileSystem::DoCopyFile(const XenFileInfo& src, const XenFileInfo& dest)
    {
        bool result = false;
        if (!IsReadOnly()) {
            XenZipFilePtr srcFile = std::static_pointer_cast<XenZipFile>(FindFile(src));
            XenZipFilePtr dstFile = std::static_pointer_cast<XenZipFile>(DoOpenFile(dest, XenFile::Out));
            
            if (srcFile && dstFile) {
                dstFile->m_Data.assign(srcFile->m_Data.begin(), srcFile->m_Data.end());
                dstFile->Close();
                
                result = true;
            }
        }
        
        return result;
    }


    bool XenZipFileSystem::DoRenameFile(const XenFileInfo& src, const XenFileInfo& dest)
    {
        return false; // TODO: Filesystem, temporally not suppoted
    }


    bool XenZipFileSystem::IsFileExists(const XenFileInfo& filePath) const
    {
        return (FindFile(BasePath() + filePath.AbsolutePath()) != nullptr);
    }


    bool XenZipFileSystem::IsFile(const XenFileInfo& filePath) const
    {
        XenFilePtr file = FindFile(filePath);
        if (file) {
            return !file->FileInfo().IsDir();
        }
        return false;
    }


    bool XenZipFileSystem::IsDir(const XenFileInfo& dirPath) const
    {
        XenFilePtr file = FindFile(dirPath);
        if (file) {
            return file->FileInfo().IsDir();
        }
        return false;
    }


    XenFilePtr XenZipFileSystem::FindFile(const XenFileInfo& fileInfo) const
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