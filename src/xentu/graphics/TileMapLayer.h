#ifndef XEN_TILEMAP_LAYER_HPP
#define XEN_TILEMAP_LAYER_HPP

#include <map>
#include <string>
#include <luna/luna.hpp>

#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/ImageLayer.hpp"

#include "Tile.h"
#include "TileMapObject.h"


namespace xen
{
	constexpr int MAX_OBJECTS_OR_TILES = 65536;


	class TileMapLayer
	{
	public:
		TileMapLayer(lua_State* L);
		TileMapLayer(lua_State* L, tmx::Map& map, const tmx::Layer::Ptr& layer);
		~TileMapLayer(void);

		const tmx::Vector2u get_size() const;
		std::vector<tmx::TileLayer::Tile> get_tiles(const tmx::Layer::Ptr& layer) const;
		const int get_texture_id() const;
		const int get_object_count() const;
		const TileMapObject* get_object(const int object_index);


		int lua_get_name(lua_State* L);
		int lua_get_offset(lua_State* L);
		int lua_get_opacity(lua_State* L);
		int lua_get_size(lua_State* L);
		int lua_get_type(lua_State* L);
		int lua_get_visible(lua_State* L);
		int lua_get_object_count(lua_State* L);
		int lua_get_object(lua_State* L);
		int lua_get_property_as_string(lua_State* L);
		int lua_get_property_as_int(lua_State* L);
		int lua_get_property_as_float(lua_State* L);
		

		int lua_set_name(lua_State* L);
		int lua_set_opacity(lua_State* L);
		int lua_set_visible(lua_State* L);


		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<TileMapLayer>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMapLayer>::FunctionType methods[];

		Tile *m_tiles[MAX_OBJECTS_OR_TILES];
		int m_tile_count;
	private:
		// properties from the old m_layer
		tmx::Vector2u m_size;
		tmx::Vector2i m_offset;
		tmx::Layer::Type m_type;


		// object info.
		TileMapObject *m_objects[MAX_OBJECTS_OR_TILES];
		int m_object_count;

		// background texture.
		int m_texture_id;
		std::string m_name;
		float m_opacity;
		bool m_visible;
	};
}

#endif