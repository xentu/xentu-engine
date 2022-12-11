#pragma once

#include <SDL.h>
#include "MouseState.h"
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
			const MouseState* GetMouseState() const;

			void Reset();

			void HandleKeyboardEvent(const SDL_Event* event);
			void HandleJoystickEvent(const SDL_Event* event);
			void HandleMouseEvent(const SDL_Event* event, const Rect* screen);

		private:
			void UpdateMousePosition(int x, int y, const Rect* screen);

			/* keyboard event buffer */
			bool m_keyboard_down_events[SDL_NUM_SCANCODES];
			SDL_Keysym m_keyboard_up_events[40];
			int m_keyboard_up_events_iter = 0;

			MouseState* m_mouse_state;
	};
}