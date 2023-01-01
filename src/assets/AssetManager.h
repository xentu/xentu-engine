#pragma once

#define AUTO_FREE 1

#include <string>
#include <map>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "../graphics/Texture.h"
#include "../graphics/TextBox.h"
#include "../graphics/SpriteMap.h"
#include "../graphics/TileMap.h"

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
			int LoadTexture(const string& path);

			/**
			 * Return an asset id for loaded texture, or -1 on fail.
			 */
			int LookupTexture(const string& path);

			/**
			 * Load a ttf font into memory, and return it's asset id.
			 */
			int LoadFont(const string& path, int font_size);

			/**
			 * Load an audio sample into memory, and return it's asset id (can be used in audio manager).
			 */
			int LoadAudio(const string& path);

			/**
			 * Load a music file (ogg/flac) into memory, and return it's asset id (can be used in audio manager).
			 */
			int LoadMusic(const string& path);

			/**
			 * Load a sprite map into memory, and return it's asset id.
			 */
			int LoadSpriteMap(const string& path);

			/**
			 * Load a tilemap from a Tiled TMX file.
			 */
			int LoadTileMapTMX(const string& path, const string& working_dir);

			/**
			 * Load a shader into memory and return it's asset id.
			 */
			int LoadShader(const string& vertex_shader, const string& frag_shader);

			/**
			 * Create a textbox with specific dimensions, and return it's asset id.
			 */
			int CreateTextBox(int x, int y, int width, int height);

			/**
			 * Create a sprite map, and return it's asset id.
			 */
			int CreateSpriteMap();


			const Texture* const GetTexture(const int id) const;

			TTF_Font* const GetFont(const int id) const;
			
			TextBox* const GetTextBox(const int id) const;

			unsigned int GetShader(const int id) const;

			SpriteMap* const GetSpriteMap(const int id) const;

			TileMap* const GetTileMap(const int id) const;


			int UnloadTexture(int id);
			int UnloadFont(int id);
			int UnloadAudio(int id);
			int UnloadMusic(int id);
			int UnloadSpriteMap(int id);
			int UnloadTileMap(int id);
			int UnloadShader(int id);


			void SetTextureWrap(const int wrap_both);
			void SetTextureWrap(const int wrap_s, const int wrap_t);
			void SetTextureInterpolation(const int both);
			void SetTextureInterpolation(const int min, const int mag);


		private:
			static AssetManager* instance;

			/* textures */
			map<const int, const Texture*> m_textures;
			map<std::string, int> m_texture_lookups;
			int m_textures_iter = 0;
			int m_tex_wrap_s = GL_CLAMP_TO_BORDER;
			int m_tex_wrap_t = GL_CLAMP_TO_BORDER;
			int m_tex_min = GL_LINEAR;
			int m_tex_mag = GL_LINEAR;

			/* fonts */
			map<int, TTF_Font*> m_fonts;
			int m_fonts_iter = 0;

			/* textboxes */
			map<int, TextBox*> m_textboxes;
			int m_textboxes_iter = 0;

			/* shaders */
			map<int, const unsigned int> m_shaders;
			int m_shaders_iter = 0;

			/* sprite maps */
			map<int, SpriteMap*> m_sprite_maps;
			int m_sprite_map_iter = 0;


			map<int, TileMap*> m_tile_maps;
			int m_tile_map_iter = 0;
	};
}