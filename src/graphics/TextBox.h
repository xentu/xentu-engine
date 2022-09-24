#pragma once

#include <string>
#include <gl/glew.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

namespace xen
{
	class TextBox
	{
		public: 
			/**
			 * Create a new textbox, 
			 */
			TextBox(int x, int y, int w, int h);
			~TextBox(void);
			void SetText(TTF_Font* font, const char* text);

		public:
			bool m_text_set;
			GLuint m_texture = 0;
    		SDL_Rect m_position;
    		TTF_Font* m_font;
    		SDL_Color m_color;
			string m_text;
	};
}