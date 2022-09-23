#include "XenNativeFile.h"

namespace xen
{
	XenNativeFile::XenNativeFile(const XenFileInfo& fileInfo)
	: m_FileInfo(fileInfo)
	, m_IsReadOnly(true)
	, m_Mode(0)
	{ }


	XenNativeFile::~XenNativeFile()
	{
		Close();
	}


	const XenFileInfo& XenNativeFile::FileInfo() const
	{
		return m_FileInfo;
	}


	uint64_t XenNativeFile::Size()
	{
		if (IsOpened()) {
			uint64_t curPos = Tell();
			Seek(0, End);
			uint64_t size = Tell();
			Seek(curPos, Begin);
			return size;
		}
		return 0;
	}


	bool XenNativeFile::IsReadOnly() const
	{
		return m_IsReadOnly;
	}


	void XenNativeFile::Open(int mode)
	{
		if (IsOpened() && m_Mode == mode)
		{
			Seek(0, XenFile::Begin);
			return;
		}
		
		m_Mode = mode;
		m_IsReadOnly = true;
		
		std::ios_base::openmode open_mode = (std::ios_base::openmode)0x00;
		if (mode & XenFile::In) {
			open_mode |= std::fstream::in | std::ios_base::binary;
		}
		if (mode & XenFile::Out) {
			m_IsReadOnly = false;
			open_mode |= std::fstream::out;
		}
		if (mode & XenFile::Append) {
			m_IsReadOnly = false;
			open_mode |= std::fstream::app;
		}
		if (mode & XenFile::Truncate) {
			open_mode |= std::fstream::trunc;
		}
		m_Stream.open(FileInfo().AbsolutePath().c_str(), open_mode);
	}


	void XenNativeFile::Close()
	{
		m_Stream.close();
	}


	bool XenNativeFile::IsOpened() const
	{
		return m_Stream.is_open();
	}


	uint64_t XenNativeFile::Seek(uint64_t offset, Origin origin)
	{
		if (!IsOpened()) {
			return 0;
		}
		
		std::ios_base::seekdir way;
		if (origin == Begin) {
			way = std::ios_base::beg;
		}
		else if (origin == End) {
			way = std::ios_base::end;
		}
		else {
			way = std::ios_base::cur;
		}
		
		m_Stream.seekg(offset, way);
		m_Stream.seekp(offset, way);
		
		return Tell();
	}


	uint64_t XenNativeFile::Tell()
	{
		return static_cast<uint64_t>(m_Stream.tellg());
	}


	uint64_t XenNativeFile::Read(uint8_t* buffer, uint64_t size)
	{
		if (!IsOpened()) {
			return 0;
		}
		
		m_Stream.read (reinterpret_cast<char*>(buffer), (std::streamsize)size);
		if (m_Stream) {
			return size;
		}
		return static_cast<uint64_t>(m_Stream.gcount());
	}


	uint64_t XenNativeFile::Write(const uint8_t* buffer, uint64_t size)
	{
		if (!IsOpened() || IsReadOnly()) {
			return 0;
		}
		
		m_Stream.write (reinterpret_cast<const char*>(buffer), (std::streamsize)size);
		if (m_Stream) {
			return size;
		}
		return static_cast<uint64_t>(m_Stream.gcount());
	}
}