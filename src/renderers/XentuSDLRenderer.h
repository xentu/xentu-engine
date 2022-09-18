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
			int create_window();
			int create_window_ex(std::string title, int x, int y, int width, int height, int mode);
			int load_texture(uint8_t* buffer, uint64_t length);
			int load_font(uint8_t* buffer, uint64_t length, int font_size);
			int create_textbox(int x, int y, int width, int height);
			bool is_running();
			void prepare();
			void present();
			void exit();
			void draw_texture(int texture_id, int x, int y, int width, int height);
			void draw_sub_texture(int texture_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
			void draw_textbox(int textbox_id);
			void set_textbox_text(int textbox_id, int font_id, const char* text);
			bool KeyDown(int key_code);

		public:			
			SDL_Window* m_windows[MAX_WINDOW_COUNT];
			SDL_Renderer* m_renderer[MAX_WINDOW_COUNT];
			int m_window_count = 0;

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