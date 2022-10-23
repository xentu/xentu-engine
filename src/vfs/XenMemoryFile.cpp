//
//  XenMemoryFile.cpp
//  vfspp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "XenMemoryFile.h"
#include <cstring>

namespace xen
{
	XenMemoryFile::XenMemoryFile(const XenFileInfo& fileInfo)
	: m_FileInfo(fileInfo)
	, m_IsReadOnly(true)
	, m_IsOpened(false)
	, m_SeekPos(0)
	, m_Mode(0)
	{ }


	XenMemoryFile::~XenMemoryFile()
	{
		Close();
	}


	const XenFileInfo& XenMemoryFile::FileInfo() const
	{
		return m_FileInfo;
	}


	uint64_t XenMemoryFile::Size()
	{
		if (IsOpened()) {
			return m_Data.size();
		}
		return 0;
	}


	bool XenMemoryFile::IsReadOnly() const
	{
		return m_IsReadOnly;
	}


	void XenMemoryFile::Open(int mode)
	{
		if (IsOpened() && m_Mode == mode) {
			Seek(0, XenFile::Begin);
			return;
		}
    
		m_Mode = mode;
		m_SeekPos = 0;
		m_IsReadOnly = true;
    
		if (mode & (int)XenFile::Out) {
			m_IsReadOnly = false;
		}
		if (mode & (int)XenFile::Append) {
			m_IsReadOnly = false;
			m_SeekPos = Size() > 0 ? Size() - 1 : 0;
		}
		if (mode & (int)XenFile::Truncate) {
			m_Data.clear();
		}
		m_IsOpened = true;
	}


	void XenMemoryFile::Close()
	{
		m_IsReadOnly = true;
		m_IsOpened = false;
		m_SeekPos = 0;
	}


	bool XenMemoryFile::IsOpened() const
	{
		return m_IsOpened;
	}


	uint64_t XenMemoryFile::Seek(uint64_t offset, Origin origin)
	{
		if (!IsOpened()) {
			return 0;
		}
    
		if (origin == XenFile::Begin) {
			m_SeekPos = offset;
		}
		else if (origin == XenFile::End) {
			m_SeekPos = Size() - offset;
		}
		else {
			m_SeekPos += offset;
		}
		m_SeekPos = std::min(m_SeekPos, Size() - 1);
		return Tell();
	}


	uint64_t XenMemoryFile::Tell()
	{
		return m_SeekPos;
	}


	uint64_t XenMemoryFile::Read(uint8_t* buffer, uint64_t size)
	{
		if (!IsOpened()) {
			return 0;
		}
    
		uint64_t bufferSize = Size() - Tell();
		uint64_t maxSize = std::min(size, bufferSize);
		if (maxSize > 0) {
			memcpy(buffer, m_Data.data(), (size_t)maxSize);
		}
		else {
			return 0;
		}

		return maxSize;
	}


	uint64_t XenMemoryFile::Write(const uint8_t* buffer, uint64_t size)
	{
		if (!IsOpened() || IsReadOnly()) {
			return 0;
		}
    
		uint64_t bufferSize = Size() - Tell();
		if (size > bufferSize) {
			m_Data.resize((size_t)(m_Data.size() + (size - bufferSize)));
		}
		memcpy(m_Data.data() + Tell(), buffer, (size_t)size);
		return size;
	}
}