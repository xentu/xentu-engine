#include "XentuSDLRenderer.h"
#include "XentuSDLTextBox.h"

namespace xen
{
	XentuSDLRenderer::XentuSDLRenderer(const XentuConfig* config)
	:	XentuRenderer::XentuRenderer(config)
	{
		XEN_LOG("- Created XentuSDLRenderer.\n");
		TTF_Init();
	}


	int XentuSDLRenderer::CreateWindow()
	{
		return CreateWindowEx(
			m_config->title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_config->window.width,
			m_config->window.height,
			0);
	}


	int XentuSDLRenderer::CreateWindowEx(std::string title, int x, int y, int width, int height, int mode)
	{
		Uint32 render_flags = SDL_RENDERER_ACCELERATED;
		m_window = SDL_CreateWindow("GAME", x, y, width, height, mode);
		m_renderer = SDL_CreateRenderer(m_window, -1, render_flags);
		return 0;
	}


	int XentuSDLRenderer::LoadTexture(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);
		auto tex = SDL_CreateTextureFromSurface(m_renderer, sur);
		SDL_FreeSurface(sur);
		
		m_textures.insert(std::make_pair(m_textures_iter, tex));
		m_textures_iter++;

		return m_textures_iter - 1;
	}


	int XentuSDLRenderer::LoadFont(uint8_t* buffer, uint64_t length, int font_size)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto font = TTF_OpenFontRW(rw, 1 /* free RWops resource once open */, font_size);

		m_fonts.insert(std::make_pair(m_fonts_iter, font));
		m_fonts_iter++;

		return m_fonts_iter - 1;
	}


	int XentuSDLRenderer::CreateTextBox(int x, int y, int width, int height)
	{
		auto textbox = new XentuSDLTextBox(m_renderer, x, y, width, height);
		m_textboxes.insert(std::make_pair(m_textboxes_iter, textbox));
		m_textboxes_iter++;
		return m_textboxes_iter - 1;
	}


	bool XentuSDLRenderer::IsRunning()
	{
		bool running = true;
		SDL_Event event;

		m_keyboard_events_iter = 0;
		
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					running = false;
					break;
				case SDL_KEYDOWN:
					m_keyboard_events[m_keyboard_events_iter] = event.key.keysym;
					m_keyboard_events_iter++;
					break;
      	}
		}

		return running && !m_exiting;
	}


	void XentuSDLRenderer::Exit()
	{
		m_exiting = true;
	}


	bool XentuSDLRenderer::KeyDown(int key_code)
	{
		for (int i=0; i<m_keyboard_events_iter; i++) {
			auto evt = m_keyboard_events[i];
			if (evt.scancode == key_code) return true;
		}
		return false;
	}


	void XentuSDLRenderer::Prepare()
	{
		SDL_SetRenderDrawColor(m_renderer, clear_color_r, clear_color_g, clear_color_b, 255);
		SDL_RenderClear(m_renderer);
	}


	void XentuSDLRenderer::Present()
	{
		SDL_RenderPresent(m_renderer);
		SDL_Delay(1000 / 60);
	}


	void XentuSDLRenderer::DrawTexture(int texture_id, int x, int y, int width, int height)
	{
		SDL_Texture* tex = m_textures[texture_id];
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		dest.w = width;
		dest.h = height;
		SDL_RenderCopy(m_renderer, tex, NULL, &dest);
	}


	void XentuSDLRenderer::DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		SDL_Texture* tex = m_textures[texture_id];
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
		SDL_Rect src;
		src.x = sx;
		src.y = sy;
		src.w = sw;
		src.h = sh;
		SDL_RenderCopy(m_renderer, tex, &src, &dest);
	}


	void XentuSDLRenderer::DrawTextBox(int textbox_id)
	{
		auto textbox = m_textboxes[textbox_id];
		SDL_Texture* tex = textbox->texture;
		SDL_RenderCopy(m_renderer, tex, NULL, &textbox->position);
	}
	

	void XentuSDLRenderer::SetTextBoxText(int textbox_id, int font_id, const char* text)
	{
		auto textbox = m_textboxes[textbox_id];
		auto font = m_fonts[font_id];
		textbox->SetText(font, text);
	}

	
	XentuSDLRenderer::~XentuSDLRenderer()
	{
		for (auto const& tex : m_textures)
		{
			SDL_DestroyTexture(tex.second);
		}
		m_textures.clear();

		for (auto const& tb : m_textboxes)
		{
			delete tb.second;
		}
		m_textures.clear();

		// no need to delete fonts.
		m_fonts.clear();
		TTF_Quit();
		SDL_DestroyWindow(m_window);
		XEN_LOG("- Destroyed XentuSDLRenderer\n");
	}
}