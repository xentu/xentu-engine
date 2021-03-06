#ifndef XEN_VIEWPORT_CPP
#define XEN_VIEWPORT_CPP


#include "Viewport.h"


// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}


namespace xen
{
	Viewport::Viewport(lua_State* L)
	 :
		width(0),
		height(0)
	{ }


	Viewport::Viewport(int width, int height)
	 :
		width(width),
		height(height)
	{ }


	Viewport::~Viewport()
	{
		// does nothing atm.
	}


	int Viewport::lua_get_width(lua_State* L)
	{
		lua_pushinteger(L, this->width);
		return 1;
	}


	int Viewport::lua_get_height(lua_State* L)
	{
		lua_pushinteger(L, this->height);
		return 1;
	}


	const char xen::Viewport::className[] = "Viewport";


	const Luna<Viewport>::PropertyType xen::Viewport::properties[] = {
		{"width", &Viewport::lua_get_width, nullptr },
		{"height", &Viewport::lua_get_height, nullptr },
		{0,0}
	};


	const Luna<Viewport>::FunctionType xen::Viewport::methods[] = {
		{0,0}
	};
}


#endif