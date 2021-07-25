#ifndef XEN_TILEMAP_HPP
#define XEN_TILEMAP_HPP

#include <map>
#include <string>
#include <luna/luna.hpp>

#include "TileMapLayer.h"
#include "../libraries/tmxlite/Map.hpp"

namespace xen
{
	class TileMap
	{
	public:
		TileMap(lua_State* L);
		~TileMap(void);

		void load(lua_State* L, std::string filename);

		tmx::Map* get_map();
		int lua_get_layers_count(lua_State* L);
		int lua_get_layer(lua_State* L);

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<TileMap>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMap>::FunctionType methods[];

	private:
		tmx::Map* m_map;
		TileMapLayer *m_layers[100]; // max 100 layers.
		int m_layers_count;
	};
}

#endif