#pragma once

#include <SDL.h>

#define XEN_GAMEPAD_DEAD_ZONE 8000

namespace xen
{
	class GamepadState
	{
		public:
			GamepadState(int index);
			~GamepadState();
			bool IsEnabled();
			bool IsButtonDown(int button);
			bool IsButtonUp(int button);
			void SetAdded();
			void SetRemoved();
			void SetButtonDown(const SDL_JoyButtonEvent& event);
			void SetButtonUp(const SDL_JoyButtonEvent& event);
			void SetAxisInfo(const SDL_JoyAxisEvent& info);
			void Reset();
		
		public:
			int m_axis_x;
			int m_axis_y;

		private:
			bool m_enabled;
			int m_up_events_iter;
			SDL_Joystick* m_handle;
			int m_index;

			/* mouse button event buffer */
			bool m_button_down_events[20];
			int m_button_up_events[20];
			int m_button_up_events_iter = 0;
	};
}