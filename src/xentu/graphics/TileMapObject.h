#ifndef XEN_TILEMAP_OBJECT_HPP
#define XEN_TILEMAP_OBJECT_HPP

#include <map>
#include <string>
#include <luna/luna.hpp>

#include "../libraries/tmxlite/Map.hpp"

namespace xen
{
	class TileMapObject
	{
	public:
		TileMapObject(lua_State* L);
		TileMapObject(lua_State* L, const tmx::Object object);
		~TileMapObject(void);


		int lua_get_name(lua_State* L);
		int lua_get_position(lua_State* L);
		int lua_get_size(lua_State* L);
		int lua_get_shape(lua_State* L);


		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<TileMapObject>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMapObject>::FunctionType methods[];

	private:
		const tmx::Object m_object;
	};
}

#endif