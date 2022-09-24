#include "../Globals.h"
#include "TextBox.h"
#include "Texture.h"

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace xen
{
	TextBox::TextBox(int x=0, int y=0, int w=100, int h=30)
	{
		m_position.x = x;
      m_position.y = y;
		m_position.w = w;
		m_position.h = h;
      m_color = {255, 255, 255};
      m_font = nullptr;
	}

	TextBox::~TextBox()
	{
		if (m_text_set) {
			// delete the gl texture.
			glDeleteTextures(1, &m_texture);
		}
		XEN_LOG("- Destroyed XentuTextBox\n");
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
		if (sur==NULL) {
			XEN_LOG("- Failed to render text: %s", SDL_GetError());
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

		auto mode = GL_RGB;
		if (sur->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur->pixels);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SDL_FreeSurface(sur);
	}


	void TextBox::SetColor(TTF_Font* font, int r, int g, int b)
	{
		m_color.r = static_cast<unsigned char>(b);
		m_color.g = static_cast<unsigned char>(g);
		m_color.b = static_cast<unsigned char>(r);
		Regenerate(font);
	}
}