#include <chrono>

#include "vfs/XenVirtualFileSystem.h"
#include "vfs/XenFileSystem.h"
#include "vfs/XenNativeFileSystem.h"
#include "Machine.h"

using namespace std::chrono_literals;

namespace xen
{
	Machine::Machine(size_t argc, char *argv[], Config* config)
	: arg_count(argc)
	{
		XEN_LOG("- Created XentuMachine.\n");

		// store the startup arguments (upto a maximum count)
		for (int i = 0; i<argc && i<MAX_ARGV; i++) {
			arg_values[i] = argv[i];
		}

		m_config = config;
		m_renderer = new DefaultRenderer(config);
		m_input = new InputManager();
	}


	void Machine::Run()
	{
		using clock = std::chrono::high_resolution_clock;
		auto time_start = clock::now();
		auto time_elapsed(0ns);
		
		bool running = true;

		while (running) {
			// calculate delta.
			auto now = clock::now();
			auto delta_time = std::chrono::duration<float>(now - time_start);
			auto delta_ms = delta_time.count();
			time_start = now;

			m_input->PollEvents();
			if (m_input->IsRunning() == false || m_renderer->IsRunning() == false) {
				running = false;
			}
			else {
				m_renderer->NewFrame();
				this->Trigger("update", delta_ms);
				this->Trigger("draw", delta_ms);
			}
		}
	}


	Config* Machine::GetConfig()
	{
		return m_config;
	}


	DefaultRenderer* Machine::GetRenderer()
	{
		return m_renderer;
	}


	InputManager* Machine::GetInput()
	{
		return m_input;
	}


	
	Machine::~Machine()
	{
		// shutdown the file system.
		xen::vfs_shutdown();

		delete m_renderer;

		XEN_LOG("- Destroyed XentuMachine.\n");
	}
}