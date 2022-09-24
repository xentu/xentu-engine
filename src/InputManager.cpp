#include "InputManager.h"

namespace xen
{
	InputManager::InputManager()
	: m_running(true)
	{ }

	InputManager::~InputManager()
	{ }

	void InputManager::PollEvents()
	{
		SDL_Event event;
		m_keyboard_down_events_iter = 0;
		m_keyboard_up_events_iter = 0;
		
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					m_running = false;
					break;
				case SDL_KEYDOWN:
					m_keyboard_down_events[m_keyboard_down_events_iter] = event.key.keysym;
					m_keyboard_down_events_iter++;
					break;
				case SDL_KEYUP:
					m_keyboard_up_events[m_keyboard_up_events_iter] = event.key.keysym;
					m_keyboard_up_events_iter++;
					break;
      	}
		}	
	}

	bool InputManager::KeyDown(int key_code)
	{
		for (int i=0; i<m_keyboard_down_events_iter; i++) {
			auto evt = m_keyboard_down_events[i];
			if (evt.scancode == key_code)
			{
				return true;
			}
		}
		return false;
	}


	bool InputManager::KeyUp(int key_code)
	{
		for (int i=0; i<m_keyboard_up_events_iter; i++) {
			auto evt = m_keyboard_up_events[i];
			if (evt.scancode == key_code)
			{
				return true;
			}
		}
		return false;
	}


	bool InputManager::IsRunning()
	{
		return m_running;
	}
}