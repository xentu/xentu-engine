#ifndef XEN_RENDERER_SDL_CPP
#define XEN_RENDERER_SDL_CPP

#include "XentuSDLRenderer.h"

namespace xen
{
	XentuSDLRenderer::XentuSDLRenderer(const XentuConfig* config)
	:	XentuRenderer::XentuRenderer(config)
	{
		XEN_LOG("Constructor for XentuSDLRenderer called.\n");
		TTF_Init();
	}


	int XentuSDLRenderer::create_window()
	{
		return create_window_ex(
			m_config->title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_config->window.width,
			m_config->window.height,
			0);
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


	int XentuSDLRenderer::load_texture(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);
		auto tex = SDL_CreateTextureFromSurface(m_renderer[0], sur);
		SDL_FreeSurface(sur);
		
		m_textures.insert(std::make_pair(m_textures_iter, tex));
		m_textures_iter++;

		return m_textures_iter - 1;
	}

	int XentuSDLRenderer::load_font(uint8_t* buffer, uint64_t length, int font_size)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto font = TTF_OpenFontRW(rw, 1 /* free RWops resource once open */, font_size);

		m_fonts.insert(std::make_pair(m_fonts_iter, font));
		m_fonts_iter++;

		return m_fonts_iter - 1;
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


	void XentuSDLRenderer::prepare()
	{
		SDL_Renderer* rend = m_renderer[0];
		SDL_SetRenderDrawColor(rend, clear_color_r, clear_color_g, clear_color_b, 255);
		SDL_RenderClear(rend);
	}


	void XentuSDLRenderer::present()
	{
		SDL_Renderer* rend = m_renderer[0];
		SDL_RenderPresent(rend);
		SDL_Delay(1000 / 60);
	}


	void XentuSDLRenderer::draw_texture(int texture_id, int x, int y, int width, int height)
	{
		SDL_Renderer* rend = m_renderer[0];
		SDL_Texture* tex = m_textures[texture_id];
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		dest.w = width;
		dest.h = height;
		SDL_RenderCopy(rend, tex, NULL, &dest);
	}
	
	
	XentuSDLRenderer::~XentuSDLRenderer()
	{
		for (auto const& tex : m_textures)
		{
			SDL_DestroyTexture(tex.second);
		}
		m_textures.clear();

		// no need to delete fonts.
		m_fonts.clear();
		TTF_Quit();

		for (int i=0; i<m_window_count; i++) {
			SDL_Window* win = m_windows[i];
			SDL_DestroyWindow(win);
		}

		XEN_LOG("Destroyed XentuRenderer\n");
	}
}

#endif