#ifndef XEN_TILEMAP_OBJECT_CPP
#define XEN_TILEMAP_OBJECT_CPP

#include "TileMapObject.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	TileMapObject::TileMapObject(lua_State* L) :
		has_tile(false)
	{ }


	TileMapObject::TileMapObject(lua_State* L, tmx::Object object) :
		has_tile(false),
		m_object(object)
	{
		
	}


	TileMapObject::~TileMapObject(void)
	{

	}


	int TileMapObject::lua_get_name(lua_State* L)
	{
		lua_pushstring(L, m_object.getName().c_str());
		return 1;
	}


	int TileMapObject::lua_get_position(lua_State* L)
	{
		auto vec2 = &m_object.getPosition();
		lua_pushinteger(L, vec2->x);
		lua_pushinteger(L, vec2->y);
		return 2;
	}


	int TileMapObject::lua_get_size(lua_State* L)
	{
		auto aabb = &m_object.getAABB();
		lua_pushinteger(L, aabb->width);
		lua_pushinteger(L, aabb->height);
		return 2;
	}


	int TileMapObject::lua_get_shape(lua_State* L) {
		std::string name = "Unknown";
		switch (m_object.getShape())
		{
			case tmx::Object::Shape::Rectangle: name = "Rectangle"; break;
			case tmx::Object::Shape::Ellipse: name = "Ellipse"; break;
			case tmx::Object::Shape::Point: name = "Point"; break;
			case tmx::Object::Shape::Polygon: name = "Polygon"; break;
			case tmx::Object::Shape::Polyline: name = "Polyline"; break;
			case tmx::Object::Shape::Text: name = "Text"; break;
		}
		lua_pushstring(L, name.c_str());
		return 1;
	}


	const xen::Tile TileMapObject::get_tile() const
	{
		return m_tile;
	}


	const char TileMapObject::className[] = "TileMapObject";


	const Luna<TileMapObject>::PropertyType TileMapObject::properties[] = {
		{0,0}
	};


	const Luna<TileMapObject>::FunctionType TileMapObject::methods[] = {
		method(TileMapObject, get_name, lua_get_name),
		method(TileMapObject, get_position, lua_get_position),
		method(TileMapObject, get_size, lua_get_size),
		method(TileMapObject, get_shape, lua_get_shape),
		{0,0}
	};
}

#endif