#include "Xentu.h"
#include "XentuRenderer.h"
#include "renderers/XentuSDLRenderer.h"
#include "fs/XenVirtualFileSystem.h"
#include "fs/XenFileSystem.h"
#include "fs/XenNativeFileSystem.h"
#include "XentuMachine.h"

namespace xen
{
	XentuMachine::XentuMachine(size_t argc, char *argv[], XentuConfig* config)
	: arg_count(argc)
	{
		XEN_LOG("- Created XentuMachine.\n");

		// store the startup arguments (upto a maximum count)
		for (int i = 0; i<argc && i<MAX_ARGV; i++) {
			arg_values[i] = argv[i];
		}

		m_config = config;
		m_renderer = new XentuSDLRenderer(config);
	}


	void XentuMachine::Run()
	{
		while (m_renderer->IsRunning()) {
			this->Trigger("update");
			m_renderer->Prepare();
			this->Trigger("draw");
			m_renderer->Present();
		}
	}


	XentuConfig* XentuMachine::GetConfig()
	{
		return m_config;
	}


	XentuRenderer* XentuMachine::GetRenderer()
	{
		return m_renderer;
	}

	
	XentuMachine::~XentuMachine()
	{
		// shutdown the file system.
		xen::vfs_shutdown();

		delete m_renderer;

		XEN_LOG("- Destroyed XentuMachine.\n");
	}
}