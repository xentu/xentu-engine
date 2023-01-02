#pragma once

#define XEN_MAX_GAMEPADS 1

#include <SDL.h>
#include "MouseState.h"
#include "GamepadState.h"
#include "../graphics/Rect.h"

namespace xen
{
	class InputManager
	{
		public:
			InputManager();
			~InputManager();
			bool KeyDown(int key_code);
			bool KeyUp(int key_code);
			bool MouseButtonDown(int button_code);
			bool MouseButtonUp(int button_code);
			const GamepadState* GetGamepadState(int index) const;
			const MouseState* GetMouseState() const;

			void Reset();

			void HandleKeyboardEvent(const SDL_Event* event);
			void HandleJoystickEvent(const SDL_Event& event);
			void HandleMouseEvent(const SDL_Event* event, const Rect* screen);

		private:
			void UpdateMousePosition(int x, int y, const Rect* screen);

			/* keyboard event buffer */
			bool m_keyboard_down_events[SDL_NUM_SCANCODES];
			SDL_Keysym m_keyboard_up_events[40];
			int m_keyboard_up_events_iter = 0;

			/* mouse button event buffer */
			bool m_mouse_button_down_events[20];
			SDL_MouseButtonEvent m_mouse_button_up_events[20];
			int m_mouse_button_up_events_iter = 0;
			MouseState* m_mouse_state;

			/* gamepad event and state buffer */
			GamepadState* m_gamepad_states[XEN_MAX_GAMEPADS];
	};
}