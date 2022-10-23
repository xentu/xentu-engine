#include "InputManager.h"

namespace xen
{
	InputManager::InputManager()
	{ 
		for (int i=0; i<SDL_NUM_SCANCODES; i++) {
			m_keyboard_down_events[i] = false;	
		}
	}

	InputManager::~InputManager()
	{ }

	bool InputManager::KeyDown(int key_code)
	{
		return m_keyboard_down_events[key_code];
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


	


	void InputManager::Reset()
	{
		m_keyboard_up_events_iter = 0;
	}

	void InputManager::SetKeyDown(int keycode)
	{
		m_keyboard_down_events[keycode] = true;
	}

	void InputManager::SetKeyUp(SDL_Keysym keysym)
	{
		m_keyboard_up_events[m_keyboard_up_events_iter] = keysym;
		m_keyboard_down_events[keysym.scancode] = false;
		m_keyboard_up_events_iter++;
	}
}