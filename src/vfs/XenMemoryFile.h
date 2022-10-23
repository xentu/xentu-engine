#pragma once

#include "XenFile.h"

namespace xen
{
	CLASS_PTR(XenFile)

	class XenMemoryFile final : public XenFile
	{
   	friend class XenMemoryFileSystem;

	public:
		XenMemoryFile(const XenFileInfo& fileInfo);
		~XenMemoryFile();
		
		/* Get file information	*/
		const XenFileInfo& FileInfo() const override;
		
		/* Returns file size */
		uint64_t Size() override;
		
		/* Check is readonly filesystem */
		bool IsReadOnly() const override;
		
		/* Open file for reading/writing */
		void Open(int mode) override;
		
		/* Close file */
		void Close() override;
		
		/* Check is file ready for reading/writing */
		bool IsOpened() const override;
		
		/* Seek on a file */
		uint64_t Seek(uint64_t offset, Origin origin) override;
		
		/* Returns offset in file */
		uint64_t Tell() override;
		
		/* Read data from file to buffer */
		uint64_t Read(uint8_t* buffer, uint64_t size) override;
		
		/* Write buffer data to file */
		uint64_t Write(const uint8_t* buffer, uint64_t size) override;
    
	private:
		std::vector<uint8_t> m_Data;
		XenFileInfo m_FileInfo;
		bool m_IsReadOnly;
		bool m_IsOpened;
		uint64_t m_SeekPos;
		int m_Mode;
	};
}