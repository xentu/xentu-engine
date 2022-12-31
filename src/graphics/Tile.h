#pragma once

namespace xen
{
	struct Tile
	{
		Tile() :
			x(0),
			y(0),
			width(1),
			height(1),
			t_x(0),
			t_y(0),
			t_width(1),
			t_height(1),
			texture_id(-1)
		{ }

		Tile(int x, int y, int w, int h) :
			x(x),
			y(y),
			width(w),
			height(h),
			t_x(0),
			t_y(0),
			t_width(1),
			t_height(1),
			texture_id(-1)
		{ }


		Tile(int x, int y, int w, int h, int texture_id) :
			x(x),
			y(y),
			width(w),
			height(h),
			t_x(0),
			t_y(0),
			t_width(1),
			t_height(1),
			texture_id(texture_id)
		{ }

		int x, y, width, height, t_x, t_y, t_width, t_height;
		int texture_id;
	};
}