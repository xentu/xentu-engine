#ifndef XEN_RENDERER_SDL_CPP
#define XEN_RENDERER_SDL_CPP

#include "XentuSDLRenderer.h"

namespace xen
{
	XentuSDLRenderer::XentuSDLRenderer(const XentuConfig* config)
	:	XentuRenderer::XentuRenderer(config)
	{
		XEN_LOG("Created XentuRenderer\n");
	}


	int XentuSDLRenderer::create_window()
	{
		return create_window_ex("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	}


	int XentuSDLRenderer::create_window_ex(std::string title, int x, int y, int width, int height, int mode)
	{
		if (m_window_count >= MAX_WINDOW_COUNT) return -1;
		int result_id = m_window_count;
		Uint32 render_flags = SDL_RENDERER_ACCELERATED;
		m_windows[result_id] = SDL_CreateWindow("GAME", x, y, width, height, mode);
		m_renderer[result_id] = SDL_CreateRenderer(m_windows[result_id], -1, render_flags);
		m_window_count++;
		return result_id;
	}


	bool XentuSDLRenderer::is_running()
	{
		int running = 1;
		SDL_Event event;
		
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					running = 0;
					break;
      	}
		}

		return running;
	}


	void XentuSDLRenderer::present()
	{
		SDL_Renderer* rend = m_renderer[0];
		SDL_RenderClear(rend);
		SDL_Delay(1000 / 30);
	}
	
	
	XentuSDLRenderer::~XentuSDLRenderer()
	{
		for (int i=0; i<m_window_count; i++) {
			SDL_Window* win = m_windows[i];
			SDL_DestroyWindow(win);
		}
		XEN_LOG("Destroyed XentuRenderer\n");
	}
}

#endif