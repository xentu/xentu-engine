#ifndef XEN_FILE_H
#define XEN_FILE_H

#include "XenFileInfo.h"

namespace xen
{
	class XenFile
	{
		public:
			/* Seek modes */
			enum Origin	{
				Begin,
				End,
				Set
			};
    
	 		/* Open file mode  */
			enum FileMode {
				In = 0x01,
				Out = 0x02,
				ReadWrite = In | Out,
				Append = 0x04,
				Truncate = 0x08
			};
    
		public:
			XenFile() = default;
			~XenFile() = default;
    
			/* Get file information */
			virtual const XenFileInfo& FileInfo() const = 0;
			
			/* Returns file size */
			virtual uint64_t Size() = 0;
			
			/* Check is readonly filesystem */
			virtual bool IsReadOnly() const = 0;
    
			/* Open file for reading/writing */
			virtual void Open(int mode) = 0;

			/* Close file */
			virtual void Close() = 0;
			
			/* Check is file ready for reading/writing */
			virtual bool IsOpened() const = 0;
    
	 		/* Seek on a file */
			virtual uint64_t Seek(uint64_t offset, Origin origin) = 0;
    
			/* Returns offset in file */
			virtual uint64_t Tell() = 0;

			/* Read data from file to buffer */
			virtual uint64_t Read(uint8_t* buffer, uint64_t size) = 0;
			
			/* Write buffer data to file */
			virtual uint64_t Write(const uint8_t* buffer, uint64_t size) = 0;
    
			/* Templated alternative to Read */
			template<typename T>
			bool Read(T& value) {
				return (Read(&value, sizeof(value)) == sizeof(value));
			}
    
			/* Templated alternative to Write */
			template<typename T>
			uint64_t Write(const T& value) {
				return (Write(&value, sizeof(value)) == sizeof(value));
			}
	};


   inline bool operator ==(const XenFile& f1, const XenFile& f2) {
		return f1.FileInfo() == f2.FileInfo();
	}
    
	
	inline bool operator ==(std::shared_ptr<XenFile> f1, std::shared_ptr<XenFile> f2) {
		if (!f1 || !f2) return false;
    	return f1->FileInfo() == f2->FileInfo();
	}    
}

#endif