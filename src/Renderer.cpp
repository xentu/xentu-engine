#include "Globals.h"
#include "Config.h"
#include "Renderer.h"

namespace xen
{
	Renderer::Renderer(const Config* config)
	: m_config(config)
	{
		XEN_LOG("- Created XentuRenderer.\n");
		clear_color_r = 0;
		clear_color_g = 0;
		clear_color_b = 0;
	}


	void Renderer::SetClearColor(int r, int g, int b)
	{
		clear_color_r = r;
		clear_color_g = g;
		clear_color_b = b;
	}
	
	
	Renderer::~Renderer()
	{
		XEN_LOG("- Destroyed XentuRenderer\n");
	}
}