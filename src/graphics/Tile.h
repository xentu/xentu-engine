#pragma once

namespace xen
{
	struct Tile
	{
		Tile() :
			m_x(0),
			m_y(0),
			m_width(1),
			m_height(1),
			m_t_x(0),
			m_t_y(0),
			m_t_width(1),
			m_t_height(1),
			m_texture_id(-1)
		{ }

		Tile(int x, int y, int w, int h) :
			m_x(x),
			m_y(y),
			m_width(w),
			m_height(h),
			m_t_x(0),
			m_t_y(0),
			m_t_width(1),
			m_t_height(1),
			m_texture_id(-1)
		{ }


		Tile(int x, int y, int w, int h, int texture_id) :
			m_x(x),
			m_y(y),
			m_width(w),
			m_height(h),
			m_t_x(0),
			m_t_y(0),
			m_t_width(1),
			m_t_height(1),
			m_texture_id(texture_id)
		{ }

		int m_x, m_y, m_width, m_height, m_t_x, m_t_y, m_t_width, m_t_height;
		int m_texture_id;
	};
}