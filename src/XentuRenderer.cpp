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
		XEN_LOG("Created XentuRenderer\n");
	}


	int XentuRenderer::create_window()
	{
		return create_window_ex("Test", 0, 0, 800, 600, 0);
	}


	int XentuRenderer::create_window_ex(std::string title, int x, int y, int width, int height, int mode)
	{
		return -1;
	}


	void XentuRenderer::run()
	{

	}
	
	
	XentuRenderer::~XentuRenderer()
	{
		XEN_LOG("Destroyed XentuRenderer\n");
	}
}

#endif