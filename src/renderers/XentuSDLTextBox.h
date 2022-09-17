#ifndef XENTU_SDL_TEXTBOX
#define XENTU_SDL_TEXTBOX

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

namespace xen
{
	class XentuSDLTextBox
	{
		public: 
			XentuSDLTextBox(SDL_Renderer* renderer, int x, int y, int w, int h);
			~XentuSDLTextBox(void);
			void SetText(TTF_Font* font, const char* text);

		public:	
			SDL_Texture* texture = NULL;
    		SDL_Rect position;
    		TTF_Font* font;
    		SDL_Renderer* renderer;
    		SDL_Color color;
			string text;
	};
}

#endif