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
		// store the startup arguments (upto a maximum count)
		for (int i = 0; i<argc && i<MAX_ARGV; i++) {
			arg_values[i] = argv[i];
		}

		m_config = config;
		m_renderer = new Renderer(config);
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

			m_input->Reset();
			SDL_Event event;
			bool size_changed = false;
			// Events management
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						this->Trigger("quit");
						running = false;
						break;
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						m_input->HandleKeyboardEvent(&event);
						break;
					case SDL_JOYAXISMOTION:
					case SDL_JOYBALLMOTION:
					case SDL_JOYBATTERYUPDATED:
					case SDL_JOYBUTTONDOWN:
					case SDL_JOYBUTTONUP:
					case SDL_JOYDEVICEADDED:
					case SDL_JOYDEVICEREMOVED:
					case SDL_JOYHATMOTION:
						m_input->HandleJoystickEvent(&event);
						break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
					case SDL_MOUSEMOTION:
					case SDL_MOUSEWHEEL:
						m_input->HandleMouseEvent(&event, &m_renderer->drawn_screen);
					case SDL_WINDOWEVENT:
						switch (event.window.event) {
							case SDL_WINDOWEVENT_RESIZED:
							case SDL_WINDOWEVENT_SIZE_CHANGED:
								size_changed = true;
								this->Trigger("window_resized");
								break;
							case SDL_WINDOWEVENT_SHOWN:
								this->Trigger("window_shown");
								break;
							case SDL_WINDOWEVENT_HIDDEN:
								this->Trigger("window_hidden");
								break;
							case SDL_WINDOWEVENT_MOVED:
								this->Trigger("window_moved");
								break;
							case SDL_WINDOWEVENT_MINIMIZED:
								this->Trigger("window_minimized");
								break;
							case SDL_WINDOWEVENT_MAXIMIZED:
								this->Trigger("window_maximized");
								break;
							case SDL_WINDOWEVENT_RESTORED:
								this->Trigger("window_restored");
								break;
							case SDL_WINDOWEVENT_ENTER:
								this->Trigger("window_enter");
								break;
							case SDL_WINDOWEVENT_LEAVE:
								this->Trigger("window_leave");
								break;
							case SDL_WINDOWEVENT_FOCUS_GAINED:
								this->Trigger("window_focus");
								break;
							case SDL_WINDOWEVENT_FOCUS_LOST:
								this->Trigger("window_blur");
								break;
						}
						break;						
				}
			}

			if (running == false || m_renderer->IsRunning() == false) {
				running = false;
			}
			else {
				m_renderer->NewFrame(size_changed);
				this->Trigger("update", delta_ms);
				this->Trigger("draw", delta_ms);
			}
		}
	}


	Config* Machine::GetConfig()
	{
		return m_config;
	}


	Renderer* Machine::GetRenderer()
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
	}
}