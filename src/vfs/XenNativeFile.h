#ifndef XEN_NATIVE_FILE_H
#define XEN_NATIVE_FILE_H

#include "XenFile.h"

namespace xen
{
	class XenNativeFile final : public XenFile
	{
	public:
		XenNativeFile(const XenFileInfo& fileInfo);
		~XenNativeFile();
    
		/* Get file information */
		virtual const XenFileInfo& FileInfo() const override;
		
		/* Returns file size */
		virtual uint64_t Size() override;
    
		/* Check is readonly filesystem */
		virtual bool IsReadOnly() const override;
    
		/* Open file for reading/writing */
		virtual void Open(int mode) override;
    
		/* Close file */
		virtual void Close() override;
    
		/* Check is file ready for reading/writing */
		virtual bool IsOpened() const override;
    
		/* Seek on a file */
		virtual uint64_t Seek(uint64_t offset, Origin origin) override;
		
		/* Returns offset in file */
		virtual uint64_t Tell() override;
    
		/* Read data from file to buffer */
		virtual uint64_t Read(uint8_t* buffer, uint64_t size) override;

		/* Write buffer data to file */
		virtual uint64_t Write(const uint8_t* buffer, uint64_t size) override;

	private:
		XenFileInfo m_FileInfo;
		std::fstream m_Stream;
		bool m_IsReadOnly;
		int m_Mode;
	};   
}

#endif