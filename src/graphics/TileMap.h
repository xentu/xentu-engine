#pragma once

#include <map>
#include <string>
#include <luna/luna.hpp>

#include <lib/tmxlite/Map.hpp>
#include "TileMapLayer.h"

namespace xen
{
	class TileMap
	{
	public:
		TileMap(lua_State* L);
		~TileMap(void);

		void load(std::string filename);

		const tmx::Map* get_map() const;
		
		int get_layers_count();

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua.
		static const Luna<TileMap>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMap>::FunctionType methods[];

	private:
		tmx::Map* m_map;
		TileMapLayer *m_layers[100]; // max 100 layers.
		int m_layers_count;
	};
}