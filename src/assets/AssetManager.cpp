#include "AssetManager.h"
#include "../Globals.h"
#include "../audio/AudioManager.h"

namespace xen
{
	AssetManager::AssetManager()
	{
		// keep a pointer to this instance.
		if (instance != nullptr) {
			printf("Error, tried to create more than one AssetManager!");
			exit(111);
		}
		instance = this;

		TTF_Init();
	}

	AssetManager::~AssetManager()
	{
		for (auto const& tex : m_textures)
		{
			delete tex.second;
		}
		m_textures.clear();

		for (auto const& tb : m_textboxes)
		{
			delete tb.second;
		}
		m_textures.clear();

		// no need to delete fonts.
		m_fonts.clear();
		TTF_Quit();
		instance = NULL;
	}

	AssetManager* AssetManager::instance = 0;
	AssetManager* AssetManager::GetInstance()
	{
		if (instance == 0)
		{
			printf("ERROR");
			exit(123);
		}
		return instance;
	}

	int AssetManager::LoadTexture(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);

		GLuint texture_id = 0;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		auto mode = GL_RGB;
		if (sur->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mode, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur->pixels);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(sur);

		Texture* texture = new Texture(texture_id, sur->w, sur->h, mode);
		
		m_textures.insert(std::make_pair(m_textures_iter, texture));
		m_textures_iter++;

		return m_textures_iter - 1;
	}

	int AssetManager::LoadFont(uint8_t* buffer, uint64_t length, int font_size)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto font = TTF_OpenFontRW(rw, 1 /* free RWops resource once open */, font_size);

		m_fonts.insert(std::make_pair(m_fonts_iter, font));
		m_fonts_iter++;

		return m_fonts_iter - 1;
	}

	int AssetManager::LoadAudio(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto audio = Mix_LoadWAV_RW(rw, 1 /* free RWops resource once open */);
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->StoreSample(audio);
	}

	int AssetManager::LoadMusic(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto audio = Mix_LoadMUS_RW(rw, 1 /* free RWops resource once open */);
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->StoreMusic(audio);
	}

	int AssetManager::CreateTextBox(int x, int y, int width, int height)
	{
		auto textbox = new TextBox(x, y, width, height);
		m_textboxes.insert(std::make_pair(m_textboxes_iter, textbox));
		m_textboxes_iter++;
		return m_textboxes_iter - 1;
	}


	Texture* AssetManager::GetTexture(int id)
	{
		return m_textures[id];
	}

	TTF_Font* AssetManager::GetFont(int id)
	{
		return m_fonts[id];
	}
			
	TextBox* AssetManager::GetTextBox(int id)
	{
		return m_textboxes[id];
	}
}