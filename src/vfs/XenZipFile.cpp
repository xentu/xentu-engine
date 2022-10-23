#include "XenZipFile.h"
#include <miniz/miniz.h>
#include <sys/stat.h>
#include <cstring>
#include "../Globals.h"
#include "../StringUtils.h"



#ifndef S_IWRITE
#   define S_IWRITE 0x0080 // Write permission, owner
#endif

/* Write user permission */
#ifndef S_IWUSR
#	define S_IWUSR S_IWRITE
#endif


namespace xen
{
    XenZip::TEntriesMap XenZip::s_Entries;


    XenZip::XenZip(const std::string& zipPath)
    : m_FileName(zipPath)
    {
        m_ZipArchive = static_cast<mz_zip_archive*>(malloc(sizeof(struct mz_zip_archive_tag)));
        memset(m_ZipArchive, 0, sizeof(struct mz_zip_archive_tag));
        
        mz_bool status = mz_zip_reader_init_file((mz_zip_archive*)m_ZipArchive, zipPath.c_str(), 0);
        if (!status)
        {
            XEN_ERROR("> Cannot open zip file: %s\n", zipPath.c_str());
            assert("Cannot open zip file" && false);
        }
        
        for (mz_uint i = 0; i < mz_zip_reader_get_num_files((mz_zip_archive*)m_ZipArchive); i++)
        {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat((mz_zip_archive*)m_ZipArchive, i, &file_stat))
            {
                XEN_ERROR("> Cannot read entry with index: %d from zip archive", i, zipPath.c_str());
                continue;
            }
            
            s_Entries[file_stat.m_filename] = std::make_tuple(file_stat.m_file_index, file_stat.m_uncomp_size);
        }
    }


    XenZip::~XenZip()
    {
        free(m_ZipArchive);
    }


    const std::string& XenZip::FileName() const
    {
        return m_FileName;
    }


    bool XenZip::MapFile(const std::string &filename, std::vector<uint8_t>& data)
    {
        TEntriesMap::const_iterator it = s_Entries.find(filename);
        if (it == s_Entries.end()) {
            return false;
        }
        
        uint32_t index = std::get<0>(it->second);
        uint64_t size = std::get<1>(it->second);
        data.resize((size_t)size);
        
        bool ok = mz_zip_reader_extract_to_mem_no_alloc((mz_zip_archive*)m_ZipArchive,
                                                        index,
                                                        data.data(),
                                                        (size_t)size,
                                                        0, 0, 0);
        return ok;
    };


    bool XenZip::IsReadOnly() const
    {
        struct stat fileStat;
        if (stat(FileName().c_str(), &fileStat) < 0) {
            return false;
        }
        return (fileStat.st_mode & S_IWUSR);
    }


    XenZipFile::XenZipFile(const XenFileInfo& fileInfo, XenZipPtr zipFile)
    : m_ZipArchive(zipFile)
    , m_FileInfo(fileInfo)
    , m_IsReadOnly(true)
    , m_IsOpened(false)
    , m_HasChanges(false)
    , m_SeekPos(0)
    , m_Mode(0)
    {
        assert(m_ZipArchive && "Cannot init zip file from empty zip archive");
    }


    XenZipFile::~XenZipFile()
    {
        
    }


    const XenFileInfo& XenZipFile::FileInfo() const
    {
        return m_FileInfo;
    }


    uint64_t XenZipFile::Size()
    {
        if (IsOpened())
        {
            return m_Data.size();
        }
        
        return 0;
    }


    bool XenZipFile::IsReadOnly() const
    {
        assert(m_ZipArchive && "Zip archive is epty");
        return (m_ZipArchive && m_ZipArchive->IsReadOnly() && m_IsReadOnly);
    }


    void XenZipFile::Open(int mode)
    {
        // TODO: ZIPFS - Add implementation of readwrite mode
        if ((mode & XenFile::Out) ||
            (mode & XenFile::Append)) {
            XEN_WARN("> Files from zip can be opened in read only mode");
            return;
        }
        
        if (!FileInfo().IsValid() ||
            (IsOpened() && m_Mode == mode) ||
            !m_ZipArchive)
        {
            return;
        }
        
        std::string absPath = FileInfo().AbsolutePath();
        if (StringUtils::StartsWith(absPath, "/"))
        {
            absPath = absPath.substr(1, absPath.length() - 1);
        }
        
        bool ok = m_ZipArchive->MapFile(absPath, m_Data);
        if (!ok) {
            auto fn = m_ZipArchive->FileName();
            XEN_ERROR("> Cannot open file: %s from zip: %s", absPath.c_str(), fn.c_str());
            return;
        }
        
        m_Mode = mode;
        m_IsReadOnly = true;
        m_SeekPos = 0;
        if (mode & XenFile::Out)
        {
            m_IsReadOnly = false;
        }
        if (mode & XenFile::Append)
        {
            m_IsReadOnly = false;
            m_SeekPos = Size() > 0 ? Size() - 1 : 0;
        }
        if (mode & XenFile::Truncate)
        {
            m_Data.clear();
        }
        
        m_IsOpened = true;
    }


    void XenZipFile::Close()
    {
        if (IsReadOnly() || !m_HasChanges)
        {
            m_IsOpened = false;
            return;
        }
        
        // TODO: ZIPFS - Add implementation of readwrite mode
        
        m_IsOpened = false;
    }


    bool XenZipFile::IsOpened() const
    {
        return m_IsOpened;
    }


    uint64_t XenZipFile::Seek(uint64_t offset, Origin origin)
    {
        if (!IsOpened())
        {
            return 0;
        }
        
        if (origin == XenFile::Begin)
        {
            m_SeekPos = offset;
        }
        else if (origin == XenFile::End)
        {
            m_SeekPos = Size() - offset;
        }
        else
        {
            m_SeekPos += offset;
        }
        m_SeekPos = std::min(m_SeekPos, Size() - 1);
        
        return Tell();
    }


    uint64_t XenZipFile::Tell()
    {
        return m_SeekPos;
    }


    uint64_t XenZipFile::Read(uint8_t* buffer, uint64_t size)
    {
        if (!IsOpened())
        {
            return 0;
        }
        
        uint64_t bufferSize = Size() - Tell();
        uint64_t maxSize = std::min(size, bufferSize);
        if (maxSize > 0)
        {
            memcpy(buffer, m_Data.data(), (size_t)maxSize);
        }
        else
        {
            return 0;
        }
        
        return maxSize;
}


    uint64_t XenZipFile::Write(const uint8_t* buffer, uint64_t size)
    {
        if (!IsOpened() || IsReadOnly())
        {
            return 0;
        }
        
        uint64_t bufferSize = Size() - Tell();
        if (size > bufferSize)
        {
            m_Data.resize((size_t)(m_Data.size() + (size - bufferSize)));
        }
        memcpy(m_Data.data() + Tell(), buffer, (size_t)size);
        
        m_HasChanges = true;
        
        return size;
    }
}