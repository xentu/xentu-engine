#pragma once

#include <SDL.h>

namespace xen
{
	class InputManager
	{
		public:
			InputManager();
			~InputManager();
			bool KeyDown(int key_code);
			bool KeyUp(int key_code);

			void Reset();
			void SetKeyDown(int keycode);
			void SetKeyUp(SDL_Keysym keysym);

		private:
			/* keyboard event buffer */
			bool m_keyboard_down_events[SDL_NUM_SCANCODES];
			SDL_Keysym m_keyboard_up_events[40];
			int m_keyboard_up_events_iter = 0;
	};
}