#include "InputManager.h"

namespace xen
{
	InputManager::InputManager()
	{ 
		for (int i=0; i<SDL_NUM_SCANCODES; i++) {
			m_keyboard_down_events[i] = false;	
		}
		for (int i=0; i<20; i++) {
			m_mouse_button_down_events[i] = false;	
		}
		m_mouse_state = new MouseState;
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


	bool InputManager::MouseButtonDown(int button_code)
	{
		return m_mouse_button_down_events[button_code];
	}


	bool InputManager::MouseButtonUp(int key_code)
	{
		for (int i=0; i<m_mouse_button_up_events_iter; i++) {
			auto evt = m_mouse_button_up_events[i];
			if (evt.button == key_code)
			{
				return true;
			}
		}
		return false;
	}


	const MouseState* InputManager::GetMouseState() const
	{
		return m_mouse_state;
	}


	void InputManager::Reset()
	{
		m_keyboard_up_events_iter = 0;
		m_mouse_state->Reset();
		m_mouse_button_up_events_iter = 0;
	}
	

	void InputManager::HandleKeyboardEvent(const SDL_Event* event)
	{
		switch (event->type) {
			case SDL_KEYDOWN:
				m_keyboard_down_events[event->key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				m_keyboard_up_events[m_keyboard_up_events_iter] = event->key.keysym;
				m_keyboard_down_events[event->key.keysym.scancode] = false;
				m_keyboard_up_events_iter++;
				break;
		}
	}


	void InputManager::HandleJoystickEvent(const SDL_Event* event)
	{
		// todo: joystick input.
	}


	void InputManager::HandleMouseEvent(const SDL_Event* event, const Rect* screen)
	{
		switch (event->type) {
			case SDL_MOUSEMOTION:
				UpdateMousePosition(
					static_cast<int>(event->button.x),
					static_cast<int>(event->button.y),
					screen
				);
			case SDL_MOUSEBUTTONDOWN:
				m_mouse_button_down_events[event->button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				m_mouse_button_up_events[m_mouse_button_up_events_iter] = event->button;
				m_mouse_button_down_events[event->button.button] = false;
				m_mouse_button_up_events_iter++;
				break;
			//case SDL_MOUSEMOTION:
			//case SDL_MOUSEWHEEL:			
		}
	}


	void InputManager::UpdateMousePosition(int x, int y, const Rect* screen)
	{
		int _x = x - static_cast<int>(screen->left);
		int _y = y - static_cast<int>(screen->top);

		// clamp the values.
		if (_x < 0) _x = 0;
		if (_x > screen->width) _x = static_cast<int>(screen->width);
		if (_y < 0) _y = 0;
		if (_y > screen->height) _y = static_cast<int>(screen->height);

		// update.
		m_mouse_state->m_x = _x;
		m_mouse_state->m_y = _y;
	}
}