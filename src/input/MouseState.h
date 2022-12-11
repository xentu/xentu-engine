#pragma once

namespace xen
{
	class MouseState
	{
		public:
			MouseState();
			void Reset();
			int m_x;
			int m_y;
	};
}