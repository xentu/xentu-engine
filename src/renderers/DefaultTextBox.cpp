#include "../Globals.h"
#include "DefaultTextBox.h"

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace xen
{
	DefaultTextBox::DefaultTextBox(SDL_Renderer* renderer, int x=0, int y=0, int w=100, int h=30)
	{
		position.x = x;
      position.y = y;
		position.w = w;
		position.h = h;
      this->color = {255, 255, 255};
      this->font = nullptr;
      this->renderer = renderer;
	}

	DefaultTextBox::~DefaultTextBox()
	{
		SDL_DestroyTexture(texture);
		XEN_LOG("- Destroyed XentuTextBox\n");
	}

	void DefaultTextBox::SetText(TTF_Font* font, const char* text)
	{
		if (this->text == text) return;
      this->text = text;
		SDL_DestroyTexture(texture);
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		if (surface==NULL) {
			XEN_LOG("- Failed to render text: %s", SDL_GetError());
		}
		position.w = surface->w;
		position.h = surface->h;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
}