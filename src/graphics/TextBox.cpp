#include "../Globals.h"
#include "TextBox.h"
#include "Texture.h"

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace xen
{
	TextBox::TextBox(int w, int h, SDL_Color color)
	{
		m_position.x = 0;
		m_position.y = 0;
		m_position.w = w;
		m_position.h = h;
		m_color = color;
      m_font = nullptr;
	}

	TextBox::~TextBox()
	{
		if (m_text_set) {
			// delete the gl texture.
			glDeleteTextures(1, &m_texture);
		}
	}

	void TextBox::SetText(TTF_Font* font, const char* text)
	{
		/**
		 * Todo: try to only re-create the gl texture when the size of the text
		 * is measured to be larger than the previous texture. This should give
		 * this some better performance.
		 */

		if (m_text == text) return;
      m_text = text;
		Regenerate(font);
	}


	void TextBox::Regenerate(TTF_Font* font)
	{
		SDL_Surface* sur = TTF_RenderUTF8_Blended(font, m_text.c_str(), m_color);
		if (sur == nullptr || sur == NULL) {
			XEN_ERROR("> Failed to render text: %s\n", SDL_GetError());
		}
		m_position.w = sur->w;
		m_position.h = sur->h;

		if (m_text_set) {
			glDeleteTextures(1, &m_texture);
			// delete old texture.
		}

		GLuint texture_id = 0;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		m_texture = texture_id;

		auto mode = GL_RGBA;
		Uint32 rmask, gmask, bmask, amask;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		#endif

		SDL_Surface* sur_clean = SDL_CreateRGBSurface(0, sur->w, sur->h, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(sur, 0, sur_clean, 0);

		glBindTexture(GL_TEXTURE_2D, texture_id);
		//glTexImage2D(GL_TEXTURE_2D, 0, mode, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur->pixels);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur_clean->pixels);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SDL_FreeSurface(sur);
		SDL_FreeSurface(sur_clean);
	}


	void TextBox::SetColor(TTF_Font* font, SDL_Color color)
	{
		m_color = color;
		Regenerate(font);
	}


	Vector2i TextBox::MeasureText(TTF_Font* font, const char* text)
	{
		int w; int h;
		TTF_SizeText(font, text, &w, &h);
		return Vector2i(w, h);
	}
}