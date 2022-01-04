#ifndef XEN_TILEMAP_OBJECT_CPP
#define XEN_TILEMAP_OBJECT_CPP

#include "TileMapObject.h"
#include "Vector2.h"
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
		m_point_count = 0;

		auto points = object.getPoints();
		if (points.size() > 0) {
			for (const tmx::Vector2f point : points)
			{
				m_points[m_point_count] = Vector2f(point.x, point.y);
				m_point_count++;
			}
		}
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


	int TileMapObject::lua_get_point_count(lua_State* L)
	{
		lua_pushinteger(L, m_point_count);
		return 1;
	}


	int TileMapObject::lua_get_point(lua_State* L)
	{
		int point_index = lua_tointeger(L, -1);
		int max_index = m_point_count - 1;

		if (point_index < 0 || point_index > max_index) {
			Advisor::throwError("Tried to access a point on a TileMapObject wi TileLayer with an invalid index.");
			return 0;
		}
		else {
			// send it to lua.
			lua_pushnumber(L, m_points[point_index].x);
			lua_pushnumber(L, m_points[point_index].y);
			return 2;
		}
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
		{"name", &TileMapObject::lua_get_name, nullptr},
		{"shape", &TileMapObject::lua_get_shape, nullptr},
		{"x", &TileMapObject::lua_get_x, &TileMapObject::lua_set_x },
		{"y", &TileMapObject::lua_get_y, &TileMapObject::lua_set_y },
		{"width", &TileMapObject::lua_get_width, &TileMapObject::lua_set_width },
		{"height", &TileMapObject::lua_get_height, &TileMapObject::lua_set_height },
		{"point_count", &TileMapObject::lua_get_point_count, nullptr },
		{0,0}
	};


	const Luna<TileMapObject>::FunctionType TileMapObject::methods[] = {
		method(TileMapObject, get_point, lua_get_point),
		{0,0}
	};
}

#endif