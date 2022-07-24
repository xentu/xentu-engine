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
		m_windows[result_id] = SDL_CreateWindow("GAME", x, y, width, height, mode);
		m_window_count++;
		return result_id;
	}


	void XentuSDLRenderer::run()
	{
		Uint32 render_flags = SDL_RENDERER_ACCELERATED;
		auto win = m_windows[0];
    	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

		int close = 0;
		while (!close) {
			SDL_Event event;
         // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
            }
        }
        SDL_RenderClear(rend);
        SDL_Delay(1000 / 30);
		}
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