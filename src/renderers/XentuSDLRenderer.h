#ifndef XEN_RENDERER_SDL
#define XEN_RENDERER_SDL

#define MAX_WINDOW_COUNT 1
#define AUTO_FREE 1

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "../XentuRenderer.h"
#include "XentuSDLTextBox.h"

namespace xen
{
	class XentuSDLRenderer : public XentuRenderer
	{
		public:
			XentuSDLRenderer(const XentuConfig* config);
			~XentuSDLRenderer();
			int CreateWindow();
			int CreateWindowEx(std::string title, int x, int y, int width, int height, int mode);
			int LoadTexture(uint8_t* buffer, uint64_t length);
			int LoadFont(uint8_t* buffer, uint64_t length, int font_size);
			int CreateTextBox(int x, int y, int width, int height);
			bool IsRunning();
			void Prepare();
			void Present();
			void Exit();
			void DrawTexture(int texture_id, int x, int y, int width, int height);
			void DrawSubTexture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
			void DrawTextBox(int textbox_id);
			void SetTextBoxText(int textbox_id, int font_id, const char* text);
			bool KeyDown(int key_code);

		public:			
			SDL_Window* m_window;
			SDL_Renderer* m_renderer;

		private:
			std::map<int, SDL_Texture*> m_textures;
			int m_textures_iter = 0;

			std::map<int, TTF_Font*> m_fonts;
			int m_fonts_iter = 0;

			std::map<int, XentuSDLTextBox*> m_textboxes;
			int m_textboxes_iter = 0;

			SDL_Keysym m_keyboard_events[40];
			int m_keyboard_events_iter = 0;

			bool m_exiting = false;
	};
}

#endif