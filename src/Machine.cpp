#include <chrono>

#include "Globals.h"
#include "Renderer.h"
#include "renderers/DefaultRenderer.h"
#include "vfs/XenVirtualFileSystem.h"
#include "vfs/XenFileSystem.h"
#include "vfs/XenNativeFileSystem.h"
#include "Machine.h"

using namespace std::chrono_literals;

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
		m_renderer = new DefaultRenderer(config);
	}


	void XentuMachine::Run()
	{
		using clock = std::chrono::high_resolution_clock;
		auto time_start = clock::now();
		auto time_elapsed(0ns);

		while (m_renderer->IsRunning()) {
			// calculate delta.
			auto now = clock::now();
			auto delta_time = std::chrono::duration<float>(now - time_start);
			auto delta_ms = delta_time.count();
			time_start = now;
			this->Trigger("update", delta_ms);
			m_renderer->Prepare();
			this->Trigger("draw", delta_ms);
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