#ifndef XEN_RENDERER_SDL
#define XEN_RENDERER_SDL

#define MAX_WINDOW_COUNT 1
#define AUTO_FREE 1

#include "SDL.h"
#include "SDL_image.h"
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
			int load_texture(uint8_t* buffer, uint64_t length);
			bool is_running();
			void present();
			SDL_Window* m_windows[MAX_WINDOW_COUNT];
			SDL_Renderer* m_renderer[MAX_WINDOW_COUNT];
			int m_window_count = 0;

		private:
			std::map<int, SDL_Texture*> m_textures;
			int m_textures_iter = 0;
	};
}

#endif