#ifndef XEN_ASSET_MAN_CPP
#define XEN_ASSET_MAN_CPP
#define STB_IMAGE_IMPLEMENTATION

#include <GLEW/GL/glew.h>
#include <GLFW3/glfw3.h>
#include <iostream>

#include <luna/luna.hpp>
#include <stb_image/stb_image.hpp>
#include "AssetManager.h"
#include "../utilities/Helper.h"
#include "../utilities/Advisor.h"


// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	AssetManager::AssetManager(lua_State* L)
	{
		default_texture = new Texture;
		textures_iter = 0;
		spritemaps_iter = 0;
		sounds_iter = 0;
		tilemap_iter = 0;
	}


	AssetManager::~AssetManager()
	{
		delete default_texture;

		for (auto const& asset : textures)
		{
			delete asset.second;
		}

		this->textures.empty();

		for (auto const& asset : spritemaps)
		{
			delete asset.second;
		}

		this->spritemaps.empty();
		
		for (auto const& asset : sounds)
		{
			delete asset.second;
		}

		for (int shader : shaders)
		{
			glDeleteShader(shader);
		}

		this->sounds.empty();
		
		Advisor::logInfo("Deleted instance of AssetManager.");
	}


	int AssetManager::load_texture(std::string filename_relative, unsigned int format, unsigned int wrap)
	{
		std::string filename = localize_path(filename_relative);

		int gl_format = GL_RGBA;
		switch (format) {
			case 1:
				gl_format = GL_RED;
				break;
		}

		int gl_wrap = GL_REPEAT;
		switch (wrap) {
			case 1:
				gl_wrap = GL_CLAMP_TO_EDGE;
				break;
			case 2:
				gl_wrap = GL_CLAMP_TO_BORDER;
				break;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned int texture;
		stbi_set_flip_vertically_on_load(1);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			Advisor::throwError("Failed to load texture ", filename_relative);
			return -1;
		}
		stbi_image_free(data);

		Texture* created = new Texture(texture, width, height, nrChannels);
		textures_iter++;
		Advisor::logInfo("Loaded texture [#", textures_iter, "] wh: ", width, "x", height, ", channels: ", nrChannels, ", from: ", filename);
		textures.insert(std::make_pair(textures_iter, created));
		return textures_iter;
	}


	int AssetManager::lua_load_texture(lua_State* L)
	{
		std::string filename = lua_tostring(L, -3);
		unsigned int format = lua_tointeger(L, -2);
		unsigned int wrap = lua_tointeger(L, -1);
		int id = this->load_texture(filename, format, wrap);
		lua_pushinteger(L, id);
		return 1;
	}


	int AssetManager::load_spritemap(std::string filename_relative)
	{
		std::string filename = localize_path(filename_relative);
		spritemaps_iter++;

		/* try to read the specified file. */
		SpriteMap* created = SpriteMap::parse_file(filename);
		spritemaps.insert(std::make_pair(spritemaps_iter, created));

		return spritemaps_iter;
	}


	int AssetManager::lua_load_spritemap(lua_State* L)
	{
		std::string filename = lua_tostring(L, -1);
		int id = this->load_spritemap(filename);
		lua_pushinteger(L, id);
		return 1;
	}


	int AssetManager::load_audio(std::string filename_relative)
	{
		std::string filename = localize_path(filename_relative);

		try 
		{
			sounds_iter++;
			Sound* created = new Sound(filename);
			sounds.insert(std::make_pair(sounds_iter, created));
			Advisor::logInfo("Loaded audio [#", sounds_iter, "] from: ", filename);
		}
		catch (int err)
		{
			switch (err) {
				case MA_ACCESS_DENIED:
					Advisor::throwError("Access denied when loading audio from: ", filename);
					break;
				case MA_DOES_NOT_EXIST:
					Advisor::throwError("Audio file does not exist: ", filename);
					break;
				default:
					Advisor::throwError("Could not load audio from: ", filename);
					break;
			}
			throw;
		}

		return sounds_iter;
	}


	TileMap* AssetManager::load_tilemap(lua_State* L, std::string filename_relative)
	{
		std::string filename = localize_path(filename_relative);
		tilemap_iter++;

		TileMap* tilemap = new TileMap(L);
		tilemap->load(filename);

		// add the intance to a list.

		return tilemap;
	}


	int AssetManager::lua_load_audio(lua_State* L)
	{
		std::string filename = lua_tostring(L, -1);
		int id = this->load_audio(filename);
		if (id >= 0) {
			lua_pushinteger(L, id);
			return 1;
		}
		return 0;
	}


	// Compiles an invidivual vertex or fragment shader.
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
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			Advisor::throwError("Failed to compile ", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader\n", message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}


	int AssetManager::lua_load_shader(lua_State* L)
	{
		std::string vertexShaderFile = localize_path( lua_tostring(L, -2) );
		std::string vertexShader = xen::Helper::read_text_file(vertexShaderFile);
		std::string fragmentShaderFile = localize_path( lua_tostring(L, -1) );
		std::string fragmentShader = xen::Helper::read_text_file(fragmentShaderFile);
		
		unsigned int program = glCreateProgram();
		unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		shaders.push_back(program);
		lua_pushinteger(L, program);
		Advisor::logInfo("Loaded shader [", program, "]");
		return 1;
	}


	int AssetManager::lua_load_tilemap(lua_State* L)
	{
		// todo: this isn't tested.
		std::string filename = lua_tostring(L, -1);
		TileMap* tilemap = load_tilemap(L, filename);
		lua_pushlightuserdata(L, tilemap);
		return 1;
	}


	const Texture* AssetManager::get_texture(int id)
	{
		if (textures.count(id))
		{
			Texture* found = textures[id];
			return found;
		}
		return default_texture;
	}


	SpriteMap* AssetManager::get_spritemap(int id)
	{
		if (spritemaps.count(id))
		{
			SpriteMap* found = spritemaps[id];
			return found;
		}
		return nullptr;
	}


	Sound* AssetManager::get_audio(int id)
	{
		if (sounds.count(id))
		{
			Sound* found = sounds[id];
			return found;
		}
		return nullptr;
	}


	std::string AssetManager::localize_path(std::string relative_path)
	{
		return this->base_path + '/' + relative_path;
	}


	const char AssetManager::className[] = "AssetManager";


	const Luna<AssetManager>::PropertyType AssetManager::properties[] = {
		{0,0}
	};


	const Luna<AssetManager>::FunctionType AssetManager::methods[] = {
		method(AssetManager, load_audio, lua_load_audio),
		method(AssetManager, load_shader, lua_load_shader),
		method(AssetManager, load_spritemap, lua_load_spritemap),
		method(AssetManager, load_texture, lua_load_texture),
		method(AssetManager, load_tilemap, lua_load_tilemap),
		{0,0}
	};
}

#endif