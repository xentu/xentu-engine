#pragma once

#define AUTO_FREE 1

#include <string>
#include <map>
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../graphics/Texture.h"
#include "../graphics/TextBox.h"

namespace xen
{
	class AssetManager
	{
		public:
			AssetManager();
			~AssetManager();
			static AssetManager* GetInstance();

			/**
			 * Load a texture into memory, and return it's asset id.
			 */
			int LoadTexture(uint8_t* buffer, uint64_t length);

			/**
			 * Load a ttf font into memory, and return it's asset id.
			 */
			int LoadFont(uint8_t* buffer, uint64_t length, int font_size);

			/**
			 * Create a textbox with specific dimensions, and return it's asset id.
			 */
			int CreateTextBox(int x, int y, int width, int height);


			Texture* GetTexture(int id);

			TTF_Font* GetFont(int id);
			
			TextBox* GetTextBox(int id);

		private:
			static AssetManager* instance;

			/* textures */
			map<int, Texture*> m_textures;
			int m_textures_iter = 0;

			/* fonts */
			map<int, TTF_Font*> m_fonts;
			int m_fonts_iter = 0;

			/* textboxes */
			map<int, TextBox*> m_textboxes;
			int m_textboxes_iter = 0;
	};
}