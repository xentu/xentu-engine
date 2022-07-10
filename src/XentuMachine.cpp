#ifndef XEN_ENVIRONMENT_CPP
#define XEN_ENVIRONMENT_CPP

#include <stdio.h>
#include <string>
#include "XentuMachine.h"

#include "src/Xentu.h"
#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenNativeFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

namespace xen
{
	XentuMachine::XentuMachine(const size_t argc, const char *argv[])
	: arg_count(argc)
	{
		// store the startup arguments (upto a maximum count)
		for (int i = 0; i<argc && i<MAX_ARGV; i++) {
			arg_values[i] = argv[i];
		}

		// initialize the file system.
		vfs_initialize();
		
		// create a native pointing to the current dir (and a test zip endpoint).
		XenFileSystemPtr root_fs(new XenNativeFileSystem("./assets"));
		XenFileSystemPtr zip_fs(new XenZipFileSystem("./assets/test.zip", "/"));
    	root_fs->Initialize();
		zip_fs->Initialize();
		
		// add the file systems to the vfs.
		XenVirtualFileSystemPtr vfs = vfs_get_global();
		vfs->AddFileSystem("/", root_fs);
    	vfs->AddFileSystem("/zip/", zip_fs);

		XEN_LOG("Created XentuMachine\n");
	}


	int XentuMachine::run()
	{
		// todo: throw error if trying to use the default machine.
		return -1;
	}


	/**
	 * Read a text file using the vfs. Note the string returned needs to be
	 * converted to be used properly by Python.
	 */
	std::string XentuMachine::read_text_file(std::string filename)
	{
		XenVirtualFileSystemPtr vfs = xen::vfs_get_global();

		xen::XenFilePtr file = vfs->OpenFile(xen::XenFileInfo(filename), xen::XenFile::In);
		if (file && file->IsOpened())
		{
			uint64_t length = file->Size(); // not always accurate.
			uint8_t data[4096]; // create a buffer.
			uint64_t r_length = file->Read(data, length); // returns true length.
			return std::string(data, data + r_length);
		}
		
		printf("\nCouldnt load file\n");
		return "";
	}

	
	XentuMachine::~XentuMachine()
	{
		// shutdown the file system.
		xen::vfs_shutdown();

		XEN_LOG("Destroyed XentuMachine\n");
	}
}

#endif