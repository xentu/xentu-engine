#ifndef XEN_RENDERER_CPP
#define XEN_RENDERER_CPP

#include "Xentu.h"
#include "XentuConfig.h"
#include "XentuRenderer.h"

namespace xen
{
	XentuRenderer::XentuRenderer(const XentuConfig* config)
	: m_config(config)
	{
		XEN_LOG("- Created XentuRenderer.\n");
		clear_color_r = 0;
		clear_color_g = 0;
		clear_color_b = 0;
	}


	void XentuRenderer::SetClearColor(int r, int g, int b)
	{
		clear_color_r = r;
		clear_color_g = g;
		clear_color_b = b;
	}
	
	
	XentuRenderer::~XentuRenderer()
	{
		XEN_LOG("- Destroyed XentuRenderer\n");
	}
}

#endif