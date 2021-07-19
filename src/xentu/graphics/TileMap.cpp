#ifndef XEN_TILEMAP_CPP
#define XEN_TILEMAP_CPP

#include <iostream>

#include "TileMap.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	TileMap::TileMap(lua_State* L)
	{
        Advisor::logInfo("Created instance of TileMap.");
	}


    TileMap::~TileMap()
	{
        Advisor::logInfo("Deleted instance of TileMap.");
    }


	void TileMap::load(std::string filename)
	{
		map.load(filename);
	}

    const char TileMap::className[] = "TileMap";


	const Luna<TileMap>::PropertyType TileMap::properties[] = {
		{0,0}
	};


	const Luna<TileMap>::FunctionType TileMap::methods[] = {
		{0,0}
	};
}

#endif