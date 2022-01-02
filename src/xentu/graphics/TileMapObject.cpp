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
		auto aabb = &m_object.getAABB();
		x = aabb->left;
		y = aabb->top;
		width = aabb->width;
		height = aabb->height;
	}


	TileMapObject::~TileMapObject(void)
	{

	}


	int TileMapObject::lua_get_name(lua_State* L)
	{
		lua_pushstring(L, m_object.getName().c_str());
		return 1;
	}


	int TileMapObject::lua_get_x(lua_State* L)
	{
		lua_pushnumber(L, x);
		return 1;
	}


	int TileMapObject::lua_get_y(lua_State* L)
	{
		lua_pushnumber(L, y);
		return 1;
	}


	int TileMapObject::lua_get_width(lua_State* L)
	{
		lua_pushnumber(L, width);
		return 1;
	}


	int TileMapObject::lua_get_height(lua_State* L)
	{
		lua_pushnumber(L, height);
		return 1;
	}


	int TileMapObject::lua_set_x(lua_State* L)
	{
		x = lua_tonumber(L, -1);
		return 0;
	}


	int TileMapObject::lua_set_y(lua_State* L)
	{
		y = lua_tonumber(L, -1);
		return 0;
	}


	int TileMapObject::lua_set_width(lua_State* L)
	{
		width = lua_tonumber(L, -1);
		return 0;
	}


	int TileMapObject::lua_set_height(lua_State* L)
	{
		height = lua_tonumber(L, -1);
		return 0;
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
		{"x", &TileMapObject::lua_get_x, &TileMapObject::lua_set_x },
		{"y", &TileMapObject::lua_get_y, &TileMapObject::lua_set_y },
		{"width", &TileMapObject::lua_get_width, &TileMapObject::lua_set_width },
		{"height", &TileMapObject::lua_get_height, &TileMapObject::lua_set_height },
		{0,0}
	};


	const Luna<TileMapObject>::FunctionType TileMapObject::methods[] = {
		method(TileMapObject, get_name, lua_get_name),
		method(TileMapObject, get_shape, lua_get_shape),
		{0,0}
	};
}

#endif