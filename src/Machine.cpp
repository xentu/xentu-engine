#include <string>

#include "Machine.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace std::chrono_literals;

namespace xen
{
	Machine::Machine()
	{
		m_engine = Engine::GetInstance();

		// keep a pointer to this instance.
		if (instance != nullptr) {
			XEN_ERROR("Error, tried to create more than one AssetManager!");
			exit(111);
		}
		instance = this;
	}


	Machine* Machine::instance = nullptr;
	Machine* Machine::GetInstance()	{
		if (instance == nullptr) {
			XEN_ERROR("Error, called DataManager::GetInstance() before initialization!");
			exit(123);
		}
		return instance;
	}

	Config* Machine::GetConfig() const {
		return m_engine->GetConfig();
	}

	Renderer* Machine::GetRenderer() const {
		return m_engine->GetRenderer();
	}

	InputManager* Machine::GetInput() const {
		return m_engine->GetInput();
	}


	void Machine::Run()
	{
		//using clock = std::chrono::high_resolution_clock;
		using clock = std::chrono::steady_clock;
		m_time_start = clock::now();
		m_time_elapsed = 0ns;

		const auto ms = static_cast<unsigned int>(1000.0 / m_engine->GetConfig()->update_frequency);
		m_timestep = std::chrono::milliseconds(ms);

		m_running = true;

		this->Trigger("init");
	}


	void Machine::RunFrame() {
		// calculate delta.
		using clock = std::chrono::steady_clock;
		auto now = clock::now();
		auto delta_time = std::chrono::duration<float>(now - m_time_start);
		auto delta_ms = delta_time.count();
		m_time_start = now;
		m_time_elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		// check to see if the renderer has given up.
		if (m_engine->GetRenderer()->IsRunning() == false) {
			m_running = false;
		}

		// update sequence.
		if (m_engine->GetConfig()->update_frequency == m_engine->GetConfig()->draw_frequency) {
			// call update sequence same time as draw.
			m_engine->GetInput()->Reset();
			this->HandleSdlEvents();
			this->Trigger("update", delta_ms);
		}
		else {
			// call update sequence at a specific frequency.
			if (m_time_elapsed >= m_timestep) {
				m_time_elapsed -= m_timestep;
				m_engine->GetInput()->Reset();
				this->HandleSdlEvents();
				this->Trigger("update", delta_ms);
			}
		}

		// draw sequence.
		m_engine->GetRenderer()->NewFrame(m_size_changed);
		this->Trigger("draw", delta_ms);
	}


	void Machine::HandleSdlEvents() {
		SDL_Event event;

		// Events management
		while (SDL_PollEvent(&event)) {

			switch (event.type) {
				case SDL_QUIT:
					this->Trigger("quit");
					m_running = false;
					break;
				case SDL_KEYDOWN:
					m_engine->GetInput()->HandleKeyboardEvent(&event);
					// fire an event if a key click happened.
					this->Trigger("key_down", (int)event.key.keysym.scancode);
					break;
				case SDL_KEYUP:
					m_engine->GetInput()->HandleKeyboardEvent(&event);
					// fire an event if a key click happened.
					if (m_engine->GetInput()->KeyUp(event.key.keysym.scancode)) {
						this->Trigger("key_click", (int)event.key.keysym.scancode);
					}
					break;
				case SDL_JOYBUTTONDOWN:
					{
						m_engine->GetInput()->HandleJoystickEvent(event);
						std::stringstream ss; ss << std::to_string(event.jbutton.which) << "," << std::to_string(event.jbutton.button);
						this->Trigger("gamepad_button_down", ss.str());
					}
					break;
				case SDL_JOYBUTTONUP:
					m_engine->GetInput()->HandleJoystickEvent(event);
					break;
				case SDL_JOYAXISMOTION:
				case SDL_JOYBALLMOTION:
				case SDL_JOYDEVICEADDED:
				case SDL_JOYDEVICEREMOVED:
				case SDL_JOYHATMOTION:
				/* case SDL_JOYBATTERYUPDATED: */
					m_engine->GetInput()->HandleJoystickEvent(event);
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
				case SDL_MOUSEWHEEL:
					m_engine->GetInput()->HandleMouseEvent(&event, &m_engine->GetRenderer()->drawn_screen);
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
						default: ;
					}
					break;
				default: ;
			}
		}
	}


	Machine::~Machine()
	{
		instance = nullptr;
	}
}