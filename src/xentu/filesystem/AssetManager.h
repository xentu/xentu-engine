#ifndef XEN_ASSET_MAN_HPP
#define XEN_ASSET_MAN_HPP

#include <string>
#include <map>
#include <list>

#include "../graphics/Texture.h"
#include "../graphics/TileMap.h"
#include "../graphics/SpriteMap.h"
#include "../audio/Sound.h"

namespace xen {
	class AssetManager
	{
	public:
		/// <summary>
		/// Create a new asset manager instance.
		/// </summary>
		AssetManager(lua_State* L);
		~AssetManager();


		/// <summary>
		/// Load a texture given a nickname and a filename. If the texture is already loaded, return the current one.
		/// Formats supported include JPEG, PNG, TGA, BMP and GIF.
		/// </summary>
		/// <param name="id">The unique nickname to give the texture.</param>
		/// <param name="filename">The full file path of the image to load.</param>
		/// <param name="mode">The colour mode to use 0=RGB, 1=RED</param>
		/// <param name="wrap">The wrap mode to use on the texture 0=REPEAT, 1=CLAMP_TO_EDGE</param>
		/// <returns>An integer representing the generated reference.</returns>
		int load_texture(std::string filename, unsigned int format, unsigned int wrap);
		int lua_load_texture(lua_State* L);


		/// <summary>
		/// Load a sprite map to be associated with a loaded texture. This allows Xentu to render parts of a texture, useful
		/// for sprite sheet's, and also for sprite font rendering. Only one sprite map can be loaded per texture.
		/// Formats supported include JSON and TOML
		/// </summary>
		/// <param name="nickname">The name of the texture to associate with.</param>
		/// <param name="filename">The full file path of the sprite map to load.</param>
		/// <returns>An integer representing success or error code.</returns>
		int load_spritemap(std::string filename);
		int lua_load_spritemap(lua_State* L);


		/// <summary>
		/// Load audio clip giving a nickname and a filename. If the audio is already loaded, return the current one.
		/// Formats supported include WAV, MP3 and FLAC.
		/// </summary>
		/// <param name="nickname">The unique nickname to give the audio clip.</param>
		/// <param name="filename">The full file path of the audio clip to load.</param>
		/// <returns></returns>
		int load_audio(std::string filename);
		int lua_load_audio(lua_State* L);


		/// <summary>
		/// Load a custom shader to be used when rendering.
		/// </summary>
		int lua_load_shader(lua_State* L);


		/// <summary>
		/// Load a Tiled TMX map.
		/// </summary>
		TileMap* load_tilemap(lua_State* L, std::string filename_relative);
		int lua_load_tilemap(lua_State* L);


		/// <summary>
		/// Get a pointer to an already loaded texture instance.
		/// </summary>
		/// <param name="nickname">The unique nickname given to the asset.</param>
		/// <returns>A pointer to a Texture instance.</returns>
		const Texture* get_texture(int id);


		/// <summary>
		/// Get a pointer to an already loaded sprite map instancee.
		/// </summary>
		/// <param name="nickname">The unique nickname given to the asset.</param>
		/// <returns>A pointer to a SpriteMap instance.</returns>
		const SpriteMap* get_spritemap(int id);


		/// <summary>
		/// Get a pointer to an aready loaded sound instance.
		/// </summary>
		/// <param name="nickname">The unique nickname given to the asset.</param>
		/// <returns>A pointer to a Sound instance.</returns>
		Sound* get_audio(int id);


		std::string base_path;
		std::string localize_path(std::string relative_path);


		// either returns a texture_id or -1 if texture is not loaded.
		int lookup_texture(std::string relative_path);


		//Class Constants
		static const char className[];


		// List of class properties that one can set/get from Lua
		static const Luna<AssetManager>::PropertyType properties[];


		// List of class methods to make available in Lua
		static const Luna<AssetManager>::FunctionType methods[];


	private:
		/// <summary>
		/// A texture instance used when no textures have yet been loaded by the
		/// user. We are using pointers so that the textures don't go out of scope.
		/// </summary>
	    Texture* default_texture;


		/// <summary>
		/// The dictionary of loaded texture instrances.
		/// </summary>
		std::map<int, Texture*> textures;
		std::map<int, SpriteMap*> spritemaps;
		std::map<int, Sound*> sounds;
		std::map<int, TileMap*> tilemaps;
		std::list<int> shaders;


		/// <summary>
		/// Lookup tables for loaded assets.
		/// </summary>
		std::map<std::string, int> texture_lookups;


		/// <summary>
		/// The new reference arrays.
		/// </summmary>
		int textures_iter;
		int spritemaps_iter;
		int sounds_iter;
		int tilemaps_iter;
	};
}

#endif