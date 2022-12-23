#include "AssetManager.h"
#include "../Globals.h"
#include "../Config.h"
#include "../Renderer.h"
#include "../audio/AudioManager.h"

namespace xen
{
	#pragma region Static Utility Methods


	// Compiles an individual vertex or fragment shader.
	static unsigned int compile_shader(unsigned int type, const std::string& source)
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
			XEN_ERROR("Failed to compile %s shader: %s", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), "\n", message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}


	// Links and attaches a vertex and fragment shader pair.
	static unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

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
			XEN_ERROR("Error, called AssetManager::GetInstance() before initialization!");
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

		Texture* texture = new Texture(texture_id, sur->w, sur->h, mode);
		SDL_FreeSurface(sur);
		
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
		if (audio == NULL || audio == nullptr)
		{
			XEN_ERROR("Failed to load audio! SDL_mixer Error: %s\n", Mix_GetError());
			return -1;
		}
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->StoreSound(audio);
	}

	int AssetManager::LoadMusic(uint8_t* buffer, uint64_t length)
	{
		auto *rw = SDL_RWFromMem(buffer, length);
		auto audio = Mix_LoadMUS_RW(rw, 1 /* free RWops resource once open */);
		if (audio == NULL || audio == nullptr)
		{
			XEN_ERROR("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
			return -1;
		}
		auto audioMgr = AudioManager::GetInstance();
		return audioMgr->StoreMusic(audio);
	}

	int AssetManager::LoadSpriteMap(std::string const& json)
	{
		auto sprite_map = SpriteMap::parse_file(json);
		m_sprite_maps.insert(std::make_pair(m_sprite_map_iter, sprite_map));
		m_sprite_map_iter++;
		return m_sprite_map_iter - 1;
	}

	int AssetManager::LoadShader(string vertex_shader, string frag_shader)
	{
		auto res = create_shader(vertex_shader, frag_shader);
		m_shaders.insert(std::make_pair(m_shaders_iter, res));
		m_shaders_iter++;
		return m_shaders_iter - 1;
	}

	int AssetManager::CreateTextBox(int x, int y, int width, int height)
	{
		auto textbox = new TextBox(x, y, width, height);
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


	unsigned int AssetManager::GetShader(int id)
	{
		return m_shaders[id];
	}


	SpriteMap* AssetManager::GetSpriteMap(int id)
	{
		return m_sprite_maps[id];
	}


	int AssetManager::UnloadTexture(int id)
	{
		auto texture = m_textures[id];
		delete texture;
		m_textures.erase(id);
		m_textures_iter--;
		return 0;
	}

	
	int AssetManager::UnloadFont(int id)
	{
		auto font = m_fonts[id];
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
	
	
	int AssetManager::UnloadShader(int id)
	{
		auto program = m_shaders[id];
		glDeleteProgram(program);
		m_shaders.erase(id);
		m_shaders_iter--;
		return 0;
	}
}