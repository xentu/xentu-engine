#ifndef XENTU_SDL_TEXTBOX_CPP
#define XENTU_SDL_TEXTBOX_CPP

#include "XentuSDLTextBox.h"

namespace xen
{
	XentuSDLTextBox::XentuSDLTextBox(SDL_Renderer* renderer, int x=0, int y=0, int w=100, int h=30)
	{
		position.x = x;
      position.y = y;
		position.w = w;
		position.h = h;
      this->color = {255, 255, 255};
      this->font = nullptr;
      this->renderer = renderer;
	}

	XentuSDLTextBox::~XentuSDLTextBox()
	{
		SDL_DestroyTexture(texture);
	}

	void XentuSDLTextBox::SetText(TTF_Font* font, const char* text)
	{
		if (this->text == text) return;
      this->text = text;
		SDL_DestroyTexture(texture);
		SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
		if (surface==NULL) {
			printf("Failed to render text: %s", SDL_GetError());            
		}
		position.w = surface->w;
		position.h = surface->h;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
}

#endif