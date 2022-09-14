#ifndef XEN_RENDERER
#define XEN_RENDERER

#include <string>

#include "Xentu.h"
#include "XentuConfig.h"

namespace xen
{
	class XentuRenderer
	{
		public:
			XentuRenderer(const XentuConfig* config);
			~XentuRenderer();
			virtual int create_window();
			virtual int create_window_ex(std::string title, int x, int y, int width, int height, int mode);
			virtual int load_texture(uint8_t* buffer, uint64_t length);
			virtual int load_font(uint8_t* buffer, uint64_t length, int font_size);
			virtual bool is_running();
			virtual void prepare();
			virtual void present();
			virtual void draw_texture(int texture_id, int x, int y, int width, int height);
			void set_clear_color(int r, int g, int b);

		protected:
			const XentuConfig* m_config;
			int clear_color_r, clear_color_g, clear_color_b;			
	};
}

#endif