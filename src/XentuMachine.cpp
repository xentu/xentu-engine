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
	 * Demonstrates how to setup the vfs using a zip filesystem and read a text file.
	 */
	std::string XentuMachine::read_text_file(std::string file)
	{
		XenVirtualFileSystemPtr vfs = xen::vfs_get_global();

		xen::XenFilePtr zipFile = vfs->OpenFile(xen::XenFileInfo(file), xen::XenFile::In);
		if (zipFile && zipFile->IsOpened())
		{
			uint64_t length = zipFile->Size();
			char data[4096];
			zipFile->Read(reinterpret_cast<uint8_t*>(data), length);
			return std::string(data, data + length);
		}
		
		printf("\nCouldnt load file\n");
		return "";
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

		XEN_LOG("Destroyed XentuMachine\n");
	}
}

#endif