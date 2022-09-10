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
		XEN_LOG("Constructor for XentuRenderer called.\n");
	}


	int XentuRenderer::create_window()
	{
		return create_window_ex("Test", 0, 0, 800, 600, 0);
	}


	int XentuRenderer::create_window_ex(std::string title, int x, int y, int width, int height, int mode)
	{
		return -1;
	}


	int XentuRenderer::load_texture(uint8_t* buffer, uint64_t length)
	{
		return -1;
	}


	bool XentuRenderer::is_running()
	{
		return 0;
	}


	void XentuRenderer::prepare()
	{
		
	}


	void XentuRenderer::present()
	{

	}


	void XentuRenderer::draw_texture(int texture_id, int x, int y, int width, int height)
	{
		
	}
	
	
	XentuRenderer::~XentuRenderer()
	{
		XEN_LOG("Destroyed XentuRenderer\n");
	}
}

#endif