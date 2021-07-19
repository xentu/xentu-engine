#ifndef XEN_TILEMAP_HPP
#define XEN_TILEMAP_HPP

#include <map>
#include <string>
#include <luna/luna.hpp>

#include "../libraries/tmxlite/Map.hpp"

namespace xen
{
	class TileMap
	{
	public:
		TileMap(lua_State* L);
		~TileMap(void);

		void load(std::string filename);

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<TileMap>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMap>::FunctionType methods[];

	private:
		tmx::Map map;
	};
}

#endif