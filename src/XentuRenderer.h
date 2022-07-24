#ifndef XEN_RENDERER
#define XEN_RENDERER

#include "Xentu.h"
#include "XentuConfig.h"

namespace xen
{
	class XentuRenderer
	{
		public:
			XentuRenderer(const XentuConfig* config);
			~XentuRenderer();
	};
}

#endif