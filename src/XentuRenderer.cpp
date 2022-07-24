#ifndef XEN_RENDERER_CPP
#define XEN_RENDERER_CPP

#include "Xentu.h"
#include "XentuConfig.h"
#include "XentuRenderer.h"

namespace xen
{
	XentuRenderer::XentuRenderer(const XentuConfig* config)
	{
		XEN_LOG("Created XentuRenderer\n");
	}
	
	
	XentuRenderer::~XentuRenderer()
	{
		XEN_LOG("Destroyed XentuRenderer\n");
	}
}

#endif