#ifndef XEN_TILEMAP_LAYER_CPP
#define XEN_TILEMAP_LAYER_CPP

#include "TileMapLayer.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen {
	
	TileMapLayer::TileMapLayer(lua_State* L)
		: m_layer(nullptr)
	{ }

	TileMapLayer::TileMapLayer(lua_State* L, const tmx::Layer::Ptr& layer)
		: m_layer(layer)
	{ }

	
	TileMapLayer::~TileMapLayer(void)
	{
		Advisor::logInfo("Deleted instance of TileMapLayer.");
	}


	const tmx::Layer::Ptr& TileMapLayer::get_layer()
	{
		return m_layer;
	}


	int TileMapLayer::lua_get_name(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushstring(L, "james");
		}
		else {
			lua_pushstring(L, m_layer->getName().c_str());
		}
		return 1;
	}


	int TileMapLayer::lua_get_offset(lua_State* L) {
		if (m_layer == nullptr) {
			auto vec2 = m_layer->getOffset();
			lua_pushinteger(L, 0);
			lua_pushinteger(L, 0);
		}
		else {
			auto vec2 = m_layer->getOffset();
			lua_pushinteger(L, vec2.x);
			lua_pushinteger(L, vec2.y);
		}
		return 2;
	}


	int TileMapLayer::lua_get_opacity(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushnumber(L, 1.0f);
		}
		else {
			lua_pushnumber(L, m_layer->getOpacity());
		}
		return 1;
	}


	int TileMapLayer::lua_get_size(lua_State* L) {
		if (m_layer == nullptr) {
			auto vec2 = m_layer->getSize();
			lua_pushinteger(L, 0);
			lua_pushinteger(L, 0);
		}
		else {
			auto vec2 = m_layer->getSize();
			lua_pushinteger(L, vec2.x);
			lua_pushinteger(L, vec2.y);
		}
		return 2;
	}


	int TileMapLayer::lua_get_type(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushstring(L, "");
		}
		else {
			switch (m_layer->getType())
			{
				case tmx::Layer::Type::Group:
					lua_pushstring(L, "Group");
					break;
				case tmx::Layer::Type::Image:
					lua_pushstring(L, "Image");
					break;
				case tmx::Layer::Type::Object:
					lua_pushstring(L, "Object");
					break;
				case tmx::Layer::Type::Tile:
					lua_pushstring(L, "Tile");
					break;
				default:
					lua_pushstring(L, "Unknown");
					break;
			}
		}
		return 1;
	}


	int TileMapLayer::lua_get_visible(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushboolean(L, false);
		}
		else {
			lua_pushboolean(L, m_layer->getVisible());
		}
		return 1;
	}


	const char TileMapLayer::className[] = "TileMapLayer";


	const Luna<TileMapLayer>::PropertyType TileMapLayer::properties[] = {
		{0,0}
	};


	const Luna<TileMapLayer>::FunctionType TileMapLayer::methods[] = {
		method(TileMapLayer, get_name, lua_get_name),
		method(TileMapLayer, get_offset, lua_get_offset),
		method(TileMapLayer, get_opacity, lua_get_opacity),
		method(TileMapLayer, get_size, lua_get_size),
		method(TileMapLayer, get_type, lua_get_type),
		method(TileMapLayer, get_visible, lua_get_visible),
		{0,0}
	};
}


#endif