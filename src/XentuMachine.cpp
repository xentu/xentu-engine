#ifndef XEN_ENVIRONMENT_CPP
#define XEN_ENVIRONMENT_CPP

#include <stdio.h>
#include <string>

#include "Xentu.h"
#include "XentuMachine.h"
#include "fs/XenVirtualFileSystem.h"
#include "fs/XenFileSystem.h"
#include "fs/XenNativeFileSystem.h"
#include "renderers/XentuSDLRenderer.h"

namespace xen
{
	XentuMachine::XentuMachine(size_t argc, char *argv[], XentuConfig* config)
	: arg_count(argc)
	{
		XEN_LOG("Constructor for XentuMachine called.\n");

		// store the startup arguments (upto a maximum count)
		for (int i = 0; i<argc && i<MAX_ARGV; i++) {
			arg_values[i] = argv[i];
		}

		m_config = config;
		m_renderer = new XentuSDLRenderer(config);
	}


	int XentuMachine::init()
	{
		return -1;
	}


	void XentuMachine::run()
	{
		while (m_renderer->is_running()) {
			this->trigger("update");
			m_renderer->prepare();
			this->trigger("draw");
			m_renderer->present();
		}
	}


	int XentuMachine::trigger(const std::string event_name)
	{
		// triggers shouldn't effect the base class.
		return -1;
	}


	XentuConfig* XentuMachine::get_config()
	{
		return m_config;
	}


	XentuRenderer* XentuMachine::get_renderer()
	{
		return m_renderer;
	}

	
	XentuMachine::~XentuMachine()
	{
		// shutdown the file system.
		xen::vfs_shutdown();

		XEN_LOG("Destroyed XentuMachine.\n");
	}
}

#endif