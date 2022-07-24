#ifndef XEN_RENDERER_SDL
#define XEN_RENDERER_SDL

#define MAX_WINDOW_COUNT 1

#include "SDL.h"
#include "../XentuRenderer.h"

namespace xen
{
	class XentuSDLRenderer : public XentuRenderer
	{
		public:
			XentuSDLRenderer(const XentuConfig* config);
			~XentuSDLRenderer();
			int create_window();
			int create_window_ex(std::string title, int x, int y, int width, int height, int mode);
			void run();
			SDL_Window* m_windows[MAX_WINDOW_COUNT];
			int m_window_count = 0;
	};
}

#endif