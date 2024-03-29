#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Vector2.h"

using namespace std;

namespace xen
{
	class TextBox
	{
		private:
			void Regenerate(TTF_Font* font);

		public: 
			/**
			 * Create a new textbox, 
			 */
			TextBox(int w, int h, SDL_Color color, bool wrap);
			~TextBox(void);
			void SetText(TTF_Font* font, const char* text);
			void SetColor(TTF_Font* font, SDL_Color color);
			Vector2i MeasureText(TTF_Font* font, const char* text);

		public:
			bool m_text_set;
			GLuint m_texture = 0;
    		SDL_Rect m_rect;
    		TTF_Font* m_font;
    		SDL_Color m_color;
			string m_text;
			bool m_wrap;
	};
}