#include <chrono>
#include <string>

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

		unsigned int ms = static_cast<unsigned int>(1000.0 / m_config->update_frequency);
		std::chrono::milliseconds timestep(ms);
		
		m_running = true;

		this->Trigger("init");

		while (m_running) {
			// calculate delta.
			auto now = clock::now();
			auto delta_time = std::chrono::duration<float>(now - time_start);
			auto delta_ms = delta_time.count();
			time_start = now;
			time_elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

			// check to see if the renderer has given up.
			if (m_renderer->IsRunning() == false) {
				m_running = false;
			}

			
			// update sequence.
			if (m_config->update_frequency == m_config->draw_frequency) {
				// call update sequence same time as draw.
				m_input->Reset();
				this->HandleSdlEvents();
				this->Trigger("update", delta_ms);
			}
			else {
				// call update sequence at a specific frequency.
				if (time_elapsed >= timestep) {
					time_elapsed -= timestep;
					m_input->Reset();
					this->HandleSdlEvents();
					this->Trigger("update", delta_ms);
				}
			}
				
			// draw sequence.
			m_renderer->NewFrame(m_size_changed);
			this->Trigger("draw", delta_ms);
		}
	}


	void Machine::HandleSdlEvents()
	{
		SDL_Event event;
		
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					this->Trigger("quit");
					m_running = false;
					break;
				case SDL_KEYDOWN:
					m_input->HandleKeyboardEvent(&event);
					// fire an event if a key click happened.
					this->Trigger("key_down", (int)event.key.keysym.scancode);
					break;
				case SDL_KEYUP:
					m_input->HandleKeyboardEvent(&event);
					// fire an event if a key click happened.
					if (m_input->KeyUp(event.key.keysym.scancode)) {
						this->Trigger("key_click", (int)event.key.keysym.scancode);
					}
					break;
				case SDL_JOYBUTTONDOWN:
					{
						m_input->HandleJoystickEvent(event);
						std::stringstream ss; ss << std::to_string(event.jbutton.which) << "," << std::to_string(event.jbutton.button);
						this->Trigger("gamepad_button_down", ss.str());
					}
					break;
				case SDL_JOYBUTTONUP:
					m_input->HandleJoystickEvent(event);
					break;
				case SDL_JOYAXISMOTION:
				case SDL_JOYBALLMOTION:
				case SDL_JOYDEVICEADDED:
				case SDL_JOYDEVICEREMOVED:
				case SDL_JOYHATMOTION:
				/* case SDL_JOYBATTERYUPDATED: */
					m_input->HandleJoystickEvent(event);
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
				case SDL_MOUSEWHEEL:
					m_input->HandleMouseEvent(&event, &m_renderer->drawn_screen);
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							m_size_changed = true;
							this->Trigger("window_changed", "resized");
							break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							m_size_changed = true;
							this->Trigger("window_changed", "size_changed");
							break;
						case SDL_WINDOWEVENT_SHOWN:
							this->Trigger("window_changed", "shown");
							break;
						case SDL_WINDOWEVENT_HIDDEN:
							this->Trigger("window_changed", "hidden");
							break;
						case SDL_WINDOWEVENT_MOVED:
							this->Trigger("window_changed", "moved");
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
							this->Trigger("window_changed", "minimized");
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							this->Trigger("window_changed", "maximized");
							break;
						case SDL_WINDOWEVENT_RESTORED:
							this->Trigger("window_changed", "restored");
							break;
						case SDL_WINDOWEVENT_ENTER:
							this->Trigger("window_changed", "enter");
							break;
						case SDL_WINDOWEVENT_LEAVE:
							this->Trigger("window_changed", "leave");
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
							this->Trigger("window_changed", "focus");
							break;
						case SDL_WINDOWEVENT_FOCUS_LOST:
							this->Trigger("window_changed", "blur");
							break;
					}
					break;
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