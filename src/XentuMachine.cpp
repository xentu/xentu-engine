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
	/**
	 * Demonstrates how to setup the vfs using a native filesystem and write a text file.
	 */
	void write_file_test()
	{
		XenVirtualFileSystemPtr vfs = xen::vfs_get_global();
		XenFilePtr nativeFile = vfs->OpenFile(XenFileInfo("/newFile.txt"), XenFile::ReadWrite);
		if (nativeFile && nativeFile->IsOpened())
		{
			std::string data = "The quick brown fox jumps over the lazy dog.";
			char char_array[255];
			int data_len = data.length();
			data.copy(char_array, data_len);
			nativeFile->Write(reinterpret_cast<uint8_t*>(char_array), data_len);
			nativeFile->Close();
		}

		xen::vfs_shutdown();
	}


	/**
	 * Demonstrates how to setup the vfs using a zip filesystem and read a text file.
	 */
	void read_file_test()
	{
		XenVirtualFileSystemPtr vfs = xen::vfs_get_global();

		xen::XenFilePtr zipFile = vfs->OpenFile(xen::XenFileInfo("/zip/file01.txt"), xen::XenFile::In);
		if (zipFile && zipFile->IsOpened())
		{
			char data[256];
			printf("\n");
			zipFile->Read(reinterpret_cast<uint8_t*>(data), 256);
			printf("%s\n", data);
		}
		else {
			printf("\nCouldnt load file");
		}
	}


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
		XenFileSystemPtr root_fs(new XenNativeFileSystem("./"));
		XenFileSystemPtr zip_fs(new XenZipFileSystem("./assets/test.zip", "/"));
    	root_fs->Initialize();
		zip_fs->Initialize();
		
		// add the file systems to the vfs.
		XenVirtualFileSystemPtr vfs = vfs_get_global();
		vfs->AddFileSystem("/f", root_fs);
    	vfs->AddFileSystem("/zip/", zip_fs);

		XEN_LOG("\nCreated XentuMachine");
	}


	int XentuMachine::run()
	{
		// todo: throw error if trying to use the default machine.
		return -1;
	}


	void XentuMachine::set_global(const std::string name, const std::string value)
	{
		// todo: throw error if trying to use the default machine.
		//printf("\nSet [%s]: %s", name.c_str(), value.c_str());
	}
	
	
	XentuMachine::~XentuMachine()
	{
		// shutdown the file system.
		xen::vfs_shutdown();

		XEN_LOG("\nDestroyed XentuMachine");
	}
}

#endif