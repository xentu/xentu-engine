#ifndef XEN_ASSET_MAN_CPP
#define XEN_ASSET_MAN_CPP
#define STB_IMAGE_IMPLEMENTATION

#include <GLEW/GL/glew.h>
#include <GLFW3/glfw3.h>
#include <iostream>

#include <luna/luna.hpp>
#include <stb_image/stb_image.hpp>
#include "AssetManager.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name) {#name, &class::name}



namespace xen
{
	AssetManager::AssetManager(lua_State* L)
	{
		default_texture = new Texture;
		textures_iter = 0;
		spritemaps_iter = 0;
		sounds_iter = 0;
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

		this->sounds.empty();
		
		std::cout << "Deleted instance of AssetManager." << std::endl;
	}



	int AssetManager::__load_texture(std::string filename_relative, unsigned int format, unsigned int wrap)
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
			std::cout << "Failed to load texture " << filename_relative << std::endl;
			return -1;
		}
		stbi_image_free(data);

		Texture* created = new Texture(texture, width, height, nrChannels);
		textures_iter++;
		std::cout << "Loaded texture [#" << textures_iter << "] wh: " << width << "x" << height << ", channels: " << nrChannels << ", from: " << filename << std::endl;
		textures.insert(std::make_pair(textures_iter, created));
		return textures_iter;
	}



	int AssetManager::load_texture(lua_State* L)
	{
		unsigned int wrap = lua_tointeger(L, -1);
		unsigned int format = lua_tointeger(L, -2);
		std::string filename = lua_tostring(L, -3);
		int id = this->__load_texture(filename, format, wrap);
		lua_pushinteger(L, id);
		return 1;
	}



	int AssetManager::__load_spritemap(std::string filename_relative)
	{
		std::string filename = localize_path(filename_relative);
		spritemaps_iter++;

		/* try to read the specified file. */
		SpriteMap* created = SpriteMap::parse_file(filename);
		spritemaps.insert(std::make_pair(spritemaps_iter, created));

		return spritemaps_iter;
	}



	int AssetManager::load_spritemap(lua_State* L)
	{
		std::string filename = lua_tostring(L, -1);
		int id = this->__load_spritemap(filename);
		lua_pushinteger(L, id);
		return 1;
	}



	int AssetManager::__load_audio(std::string filename_relative)
	{
		std::string filename = localize_path(filename_relative);

		sounds_iter++;

		std::cout << "Loaded audio [#" << sounds_iter << "] from: " << filename << std::endl;

		Sound* created = new Sound(filename);
		sounds.insert(std::make_pair(sounds_iter, created));
		return sounds_iter;
	}



	int AssetManager::load_audio(lua_State* L)
	{
		std::string filename = lua_tostring(L, -1);
		int id = this->__load_audio(filename);
		lua_pushinteger(L, id);
		return 0;
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



	int AssetManager::say_yo(lua_State* L)
	{
		std::string str = lua_tostring(L, -1);
		std::cout << "nested lua function asked c++ to say:" << str << std::endl;
		return 1;
	}



	std::string AssetManager::localize_path(std::string relative_path)
	{
		return this->base_path + relative_path;
	}



	const char AssetManager::className[] = "AssetManager";



	const Luna<AssetManager>::PropertyType AssetManager::properties[] = {
		{0,0}
	};



	const Luna<AssetManager>::FunctionType AssetManager::methods[] = {
		method(AssetManager, say_yo),
		method(AssetManager, load_texture),
		method(AssetManager, load_spritemap),
		method(AssetManager, load_audio),
		{0,0}
	};
}



#endif