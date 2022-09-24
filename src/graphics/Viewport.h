#pragma once

namespace xen
{
	struct Viewport
	{
	public:
		Viewport(int width, int height, int mode);
		~Viewport(void);
		
		int width;
		int height;
		int mode;
	};
}