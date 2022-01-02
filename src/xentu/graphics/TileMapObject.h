#ifndef XEN_TILEMAP_OBJECT_HPP
#define XEN_TILEMAP_OBJECT_HPP

#include <map>
#include <string>
#include <luna/luna.hpp>

#include "../libraries/tmxlite/Map.hpp"
#include "Tile.h"

namespace xen
{
	class TileMapObject
	{
	public:
		TileMapObject(lua_State* L);
		TileMapObject(lua_State* L, const tmx::Object object);
		~TileMapObject(void);


		int lua_get_name(lua_State* L);

		int lua_get_x(lua_State* L);
		int lua_get_y(lua_State* L);
		int lua_get_width(lua_State* L);
		int lua_get_height(lua_State* L);

		int lua_set_x(lua_State* L);
		int lua_set_y(lua_State* L);
		int lua_set_width(lua_State* L);
		int lua_set_height(lua_State* L);

		int lua_get_shape(lua_State* L);

		const xen::Tile get_tile() const;

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<TileMapObject>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMapObject>::FunctionType methods[];


		bool has_tile;
		float x;
		float y;
		float width;
		float height;
		xen::Tile m_tile;

	private:
		const tmx::Object m_object;

	};
}

#endif