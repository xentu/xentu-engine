#include "AssetManager.h"
#include "../Globals.h"
#include "../Config.h"
#include "../Renderer.h"
#include "../audio/AudioManager.h"
#include "../vfs/XenVirtualFileSystem.h"
#include "../vfs/XenFileSystem.h"
#include "../vfs/XenZipFileSystem.h"

#define TEX_LINEAR 0
#define TEX_NEAREST 1
#define TEX_CLAMP_TO_EDGE 2
#define TEX_CLAMP_TO_BORDER 3
#define TEX_MIRRORED_REPEAT 4
#define TEX_REPEAT 5

namespace xen
{
	#pragma region Static Utility Methods


	// Compiles an individual vertex or fragment shader.
	static unsigned int compile_shader(unsigned int type, const string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		/* handle errors */
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			#if defined(WIN32) || defined(_WIN32) 
			char* message = (char*)_malloca(length * sizeof(char));
			#else
			char* message = (char*)alloca(length * sizeof(char));
			#endif
			//
			glGetShaderInfoLog(id, length, &length, message);
			XEN_ERROR("Failed to compile %s shader: %s\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}


	// Links and attaches a vertex and fragment shader pair.
	static unsigned int create_shader(const string& vertexShader, const string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

		if (vs == 0 || fs == 0) return 0;

		glAttachShader(program, vs);
		glBindAttribLocation(program, 0, "i_position");
		glBindAttribLocation(program, 1, "i_texcoord");
		glBindAttribLocation(program, 2, "i_color");

		glAttachShader(program, fs);
		glLinkProgram(program);

		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked != GL_TRUE) {
			XEN_ERROR("> Failed to link shader.");
		}

		// TODO: glBindVertexArray(VAO); should be done before validation to avoid a warning on the next part.

		int valid;
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
		if (valid != GL_TRUE) {
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetProgramInfoLog(program, length, &length, &error[0]);
			XEN_ERROR(&error[0]);
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}


	#pragma endregion


	AssetManager::AssetManager()
	{
		// keep a pointer to this instance.
		if (instance != nullptr) {
			XEN_ERROR("Error, tried to create more than one AssetManager!");
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
		m_textboxes.clear();

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
			XEN_ERROR("Error, called AssetManager::GetInstance() before initialization!");
			exit(123);
		}
		return instance;
	}

	int AssetManager::LoadTexture(const string& path)
	{
		int existing_asset_id = this->LookupTexture(path);
		if (existing_asset_id >= 0) {
			return existing_asset_id;
		}

		auto res = vfs_get_global()->ReadAllData(path);
		auto *rw = SDL_RWFromMem(res.buffer, res.length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);
		if(sur == NULL)
		{
			XEN_ERROR("Error whilst loading image '%s'\n.", path.c_str());
			SDL_FreeRW(rw);
			return -1;
		}

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

		GLuint texture_id = 0;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		/*int gl_wrap = GL_REPEAT;
		switch (wrap) {
			case 1:
				gl_wrap = GL_CLAMP_TO_EDGE;
				break;
			case 2:
				gl_wrap = GL_CLAMP_TO_BORDER;
				break;
		} */
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);
 		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_tex_wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_tex_wrap_t);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_tex_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_tex_mag);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur_clean->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		Texture* texture = new Texture(texture_id, sur->w, sur->h, mode);
		XEN_ECHO("Loaded texture: %s [gl: %d] [id: %d] (Size: %d x %d).\n", path.c_str(), (int)texture_id, m_textures_iter, sur->w, sur->h);
		SDL_FreeSurface(sur);
		SDL_FreeSurface(sur_clean);
		
		m_textures.insert(std::make_pair(m_textures_iter, texture));
		m_texture_lookups.insert(std::make_pair(path, m_textures_iter));
		m_textures_iter++;

		return m_textures_iter - 1;
	}

	SDL_Surface* AssetManager::LoadTextureSurface(const string& path)
	{
		auto res = vfs_get_global()->ReadAllData(path);
		auto *rw = SDL_RWFromMem(res.buffer, res.length);
		auto sur = IMG_Load_RW(rw, AUTO_FREE);
		if(sur == NULL)
		{
			XEN_ERROR("Error whilst loading image '%s'\n.", path.c_str());
			SDL_FreeRW(rw);
			return NULL;
		}
		return sur;
	}

	int AssetManager::LookupTexture(const string& path)
	{
		std::map<std::string, int>::iterator it = m_texture_lookups.find(path);
		if (it != m_texture_lookups.end())
		{
			return it->second;
		}
		return -1;
	}

	int AssetManager::LoadFont(const string& path, int font_size)
	{
		auto res = vfs_get_global()->ReadAllData(path);
		auto *rw = SDL_RWFromMem(res.buffer, res.length);
		auto font = TTF_OpenFontRW(rw, 1 /* free RWops resource once open */, font_size);

		m_fonts.insert(std::make_pair(m_fonts_iter, font));
		m_fonts_iter++;

		return m_fonts_iter - 1;
	}

	int AssetManager::LoadAudio(const string& path)
	{
		auto res = vfs_get_global()->ReadAllData(path);
		auto *rw = SDL_RWFromMem(res.buffer, res.length);
		auto audio = Mix_LoadWAV_RW(rw, 1 /* free RWops resource once open */);
		if (audio == NULL || audio == nullptr)
		{
			XEN_ERROR("Failed to load audio! SDL_mixer Error: %s\n", Mix_GetError());
			return -1;
		}
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->StoreSound(audio);
	}

	int AssetManager::LoadMusic(const string& path)
	{
		auto res = vfs_get_global()->ReadAllData(path);
		auto *rw = SDL_RWFromMem(res.buffer, res.length);
		auto audio = Mix_LoadMUS_RW(rw, 1 /* free RWops resource once open */);
		if (audio == NULL || audio == nullptr)
		{
			XEN_ERROR("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
			return -1;
		}
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->StoreMusic(audio);
	}

	int AssetManager::LoadSpriteMap(const string& path)
	{
		const string json = vfs_get_global()->ReadAllText(path);
		auto sprite_map = SpriteMap::parse_json(json);
		m_sprite_maps.insert(std::make_pair(m_sprite_map_iter, sprite_map));
		m_sprite_map_iter++;
		return m_sprite_map_iter - 1;
	}

	int AssetManager::LoadTileMapTMX(const string& path, const string& working_dir) 
	{
		const string xml = vfs_get_global()->ReadAllText(path);
		TileMap* map = new TileMap;
		map->LoadTMX(xml, working_dir);
		m_tile_maps.insert(std::make_pair(m_tile_map_iter, map));
		m_tile_map_iter++;
		return m_tile_map_iter - 1;
	}

	int AssetManager::LoadShader(const string& vertex_shader, const string& frag_shader)
	{
		auto res = create_shader(vertex_shader, frag_shader);
		if (res == 0) return res;
		m_shaders.insert(std::make_pair(m_shaders_iter, res));
		m_shaders_iter++;
		return m_shaders_iter - 1;
	}

	int AssetManager::CreateTextBox(int width, int height, const SDL_Color& color, const bool wrap)
	{
		auto textbox = new TextBox(width, height, color, wrap);
		m_textboxes.insert(std::make_pair(m_textboxes_iter, textbox));
		m_textboxes_iter++;
		return m_textboxes_iter - 1;
	}

	int AssetManager::CreateSpriteMap()
	{
		auto sprite_map = new SpriteMap();
		m_sprite_maps.insert(std::make_pair(m_sprite_map_iter, sprite_map));
		m_sprite_map_iter++;
		return m_sprite_map_iter - 1;
	}


	const Texture* const AssetManager::GetTexture(const int id) const
	{
		return m_textures.at(id);
	}


	TTF_Font* const AssetManager::GetFont(const int id) const
	{
		return m_fonts.at(id);
	}


	TextBox* const AssetManager::GetTextBox(const int id) const
	{
		return m_textboxes.at(id);
	}


	unsigned int AssetManager::GetShader(const int id) const
	{
		return m_shaders.at(id);
	}


	SpriteMap* const AssetManager::GetSpriteMap(const int id) const
	{
		return m_sprite_maps.at(id);
	}


	TileMap* const AssetManager::GetTileMap(const int id) const
	{
		return m_tile_maps.at(id);
	}


	int AssetManager::UnloadTexture(int id)
	{
		auto texture = m_textures.at(id);
		delete texture;
		m_textures.erase(id);
		m_textures_iter--;
		return 0;
	}

	
	int AssetManager::UnloadFont(int id)
	{
		auto font = m_fonts.at(id);
		TTF_CloseFont(font);
		m_fonts.erase(id);
		m_fonts_iter--;
		return 0;
	}

	
	int AssetManager::UnloadAudio(int id)
	{
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->UnloadAudio(id);
	}
	

	int AssetManager::UnloadMusic(int id)
	{
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->UnloadMusic(id);
	}
	

	int AssetManager::UnloadSpriteMap(int id)
	{
		auto sprite_map = m_sprite_maps[id];
		auto texture_id = sprite_map->get_texture();
		// unload the texture.
		this->UnloadTexture(texture_id);
		// remove the actual sprite map.
		delete sprite_map;
		m_sprite_maps.erase(id);
		m_sprite_map_iter--;
		return 0;
	}


	int AssetManager::UnloadTileMap(int id)
	{
		auto tile_map = m_tile_maps[id];
		delete tile_map;
		m_tile_maps.erase(id);
		m_tile_map_iter--;
		return 0;
	}
	
	
	int AssetManager::UnloadShader(int id)
	{
		auto program = m_shaders[id];
		glDeleteProgram(program);
		m_shaders.erase(id);
		m_shaders_iter--;
		return 0;
	}


	void AssetManager::SetTextureWrap(const int wrap_both)
	{
		switch(wrap_both) {
			case TEX_CLAMP_TO_EDGE: m_tex_wrap_s = m_tex_wrap_t = GL_CLAMP_TO_EDGE; break;
			case TEX_CLAMP_TO_BORDER: m_tex_wrap_s = m_tex_wrap_t = GL_CLAMP_TO_BORDER; break;
			case TEX_MIRRORED_REPEAT: m_tex_wrap_s = m_tex_wrap_t = GL_MIRRORED_REPEAT; break;
			case TEX_REPEAT: m_tex_wrap_s = m_tex_wrap_t = GL_REPEAT; break;
		}
	}


	void AssetManager::SetTextureWrap(const int wrap_s, const int wrap_t)
	{
		switch(wrap_s) {
			case TEX_CLAMP_TO_EDGE: m_tex_wrap_s = GL_CLAMP_TO_EDGE; break;
			case TEX_CLAMP_TO_BORDER: m_tex_wrap_s = GL_CLAMP_TO_BORDER; break;
			case TEX_MIRRORED_REPEAT: m_tex_wrap_s = GL_MIRRORED_REPEAT; break;
			case TEX_REPEAT: m_tex_wrap_s = GL_REPEAT; break;
		}

		switch(wrap_t) {
			case TEX_CLAMP_TO_EDGE: m_tex_wrap_t = GL_CLAMP_TO_EDGE; break;
			case TEX_CLAMP_TO_BORDER: m_tex_wrap_t = GL_CLAMP_TO_BORDER; break;
			case TEX_MIRRORED_REPEAT: m_tex_wrap_t = GL_MIRRORED_REPEAT; break;
			case TEX_REPEAT: m_tex_wrap_t = GL_REPEAT; break;
		}
	}


	void AssetManager::SetTextureInterpolation(const int both)
	{
		switch(both) {
			case TEX_LINEAR: m_tex_min = m_tex_mag = GL_LINEAR; break;
			case TEX_NEAREST: m_tex_min = m_tex_mag = GL_NEAREST; break;
		}
	}


	void AssetManager::SetTextureInterpolation(const int min, const int mag)
	{
		switch(min) {
			case TEX_LINEAR: m_tex_min = GL_LINEAR; break;
			case TEX_NEAREST: m_tex_min = GL_NEAREST; break;
		}

		switch(mag) {
			case TEX_LINEAR: m_tex_mag = GL_LINEAR; break;
			case TEX_NEAREST: m_tex_mag = GL_NEAREST; break;
		}
	}
}