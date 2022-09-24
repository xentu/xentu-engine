#include "Viewport.h"


namespace xen
{
	Viewport::Viewport(int width, int height, int mode)
	 :
		width(width),
		height(height),
		mode(mode)
	{ }


	Viewport::~Viewport()
	{
		// does nothing atm.
	}
}