#ifndef XEN_ENVIRONMENT_CPP
#define XEN_ENVIRONMENT_CPP

#include <stdio.h>
#include <string>

#include "Xentu.h"
#include "XentuMachine.h"
#include "fs/XenVirtualFileSystem.h"
#include "fs/XenFileSystem.h"
#include "fs/XenNativeFileSystem.h"

namespace xen
{
	XentuMachine::XentuMachine(size_t argc, char *argv[], const XentuConfig* config)
	: arg_count(argc)
	{
		// store the startup arguments (upto a maximum count)
		for (int i = 0; i<argc && i<MAX_ARGV; i++) {
			arg_values[i] = argv[i];
		}

		m_config = config;
		XEN_LOG("Created XentuMachine\n");
	}


	int XentuMachine::run(const std::string entry_point)
	{
		// todo: throw error if trying to use the default machine.
		return -1;
	}


	int XentuMachine::trigger(const std::string event_name)
	{
		// triggers shouldn't effect the base class.
		return -1;
	}

	
	XentuMachine::~XentuMachine()
	{
		// shutdown the file system.
		xen::vfs_shutdown();

		XEN_LOG("Destroyed XentuMachine\n");
	}
}

#endif