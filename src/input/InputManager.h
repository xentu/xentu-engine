#pragma once

#include <SDL.h>

namespace xen
{
	class InputManager
	{
		public:
			InputManager();
			~InputManager();
			void PollEvents();
			bool KeyDown(int key_code);
			bool KeyUp(int key_code);
			bool IsRunning();
			bool IsSizeChanged();

		private:
			bool m_running = true;
			bool m_size_changed = false;
			/* keyboard event buffer */
			bool m_keyboard_down_events[SDL_NUM_SCANCODES];
			SDL_Keysym m_keyboard_up_events[40];
			int m_keyboard_up_events_iter = 0;
	};
}