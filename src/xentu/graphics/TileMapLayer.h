#ifndef XEN_TILEMAP_LAYER_HPP
#define XEN_TILEMAP_LAYER_HPP

#include <map>
#include <string>
#include <luna/luna.hpp>

#include "TileMapObject.h"
#include "../libraries/tmxlite/Map.hpp"

namespace xen
{
	class TileMapLayer
	{
	public:
		TileMapLayer(lua_State* L);
		TileMapLayer(lua_State* L, const tmx::Layer::Ptr& layer);
		~TileMapLayer(void);

		const tmx::Layer::Ptr& get_layer();


		int lua_get_name(lua_State* L);
		int lua_get_offset(lua_State* L);
		int lua_get_opacity(lua_State* L);
		int lua_get_size(lua_State* L);
		int lua_get_type(lua_State* L);
		int lua_get_visible(lua_State* L);
		int lua_get_objects_count(lua_State* L);
		int lua_get_object(lua_State* L);


		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<TileMapLayer>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<TileMapLayer>::FunctionType methods[];

	private:
		const tmx::Layer::Ptr& m_layer;
		TileMapObject *m_objects[1000];
		int m_objects_count;
	};
}

#endif