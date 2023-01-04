#include "GamepadState.h"

namespace xen
{
	GamepadState::GamepadState(int index)
	: m_enabled(false),
	  m_axis_x(0),
	  m_axis_y(0),
	  m_button_up_events_iter(0)
	{
		m_index = index;
	}

	GamepadState::~GamepadState()
	{
		if (m_handle != NULL) {
			SDL_JoystickClose(m_handle);
			m_handle = NULL;
		}
	}

	bool GamepadState::IsEnabled()
	{
		return m_enabled;
	}

	const bool GamepadState::IsButtonDown(int button)
	{
		return m_button_down_events[button];
	}

	const bool GamepadState::IsButtonUp(int button)
	{
		for (int i=0; i<m_button_up_events_iter; i++) {
			if (m_button_up_events[i] == button)
			{
				return true;
			}
		}
		return false;
	}
	
	void GamepadState::SetAdded()
	{
		m_handle = SDL_JoystickOpen(m_index);
		m_enabled = m_handle != NULL;
	}

	void GamepadState::SetRemoved()
	{
		SDL_JoystickClose(m_handle);
		m_handle = NULL;
	}

	void GamepadState::SetButtonDown(const SDL_JoyButtonEvent& event)
	{
		m_button_down_events[event.button] = true;
	}
	
	void GamepadState::SetButtonUp(const SDL_JoyButtonEvent& event)
	{
		m_button_down_events[event.button] = false;
		m_button_up_events[m_button_up_events_iter] = event.button;
		m_button_up_events_iter++;
	}

	void GamepadState::SetAxisInfo(const SDL_JoyAxisEvent& info)
	{
		if (info.axis == 0)
		{
			//Left of dead zone
			m_axis_x = 0;
			if(info.value < -XEN_GAMEPAD_DEAD_ZONE)
			{
				m_axis_x = -1;
			}
			//Right of dead zone
			else if(info.value > XEN_GAMEPAD_DEAD_ZONE)
			{
				m_axis_x =  1;
			}
		}
		else if (info.axis == 1)
		{
			//Left of dead zone
			m_axis_y = 0;
			if(info.value < -XEN_GAMEPAD_DEAD_ZONE)
			{
				m_axis_y = -1;
			}
			//Right of dead zone
			else if(info.value > XEN_GAMEPAD_DEAD_ZONE)
			{
				m_axis_y =  1;
			}
		}
	}
	
	void GamepadState::Reset()
	{
		m_button_up_events_iter = 0;
	}

	const int GamepadState::GetAxisX()
	{
		return m_axis_x;
	}
	
	const int GamepadState::GetAxisY()
	{
		return m_axis_y;
	}
}