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
		map = nullptr;
        Advisor::logInfo("Created instance of TileMap.");
	}


    TileMap::~TileMap()
	{
		if (map != nullptr) {
			delete map;
		}
        Advisor::logInfo("Deleted instance of TileMap.");
    }


	void TileMap::load(std::string filename)
	{
		map = new tmx::Map();
		if (map->load(filename))
		{
			const auto& layers = map->getLayers();
			std::cout << "Map has " << layers.size() << " layers" <<  std::endl;
			for (const auto& layer : layers)
			{
				std::cout << "Found Layer: " << layer->getName() << std::endl;
				std::cout << "Layer Type: " << int(layer->getType()) << std::endl;
			}
			xen::Advisor::logInfo("Loaded tmx!");
		}
		else {
			xen::Advisor::logInfo("Failed to load tmx :(");
		}
	}

	int TileMap::lua_say_hello(lua_State* L)
	{
		std::cout << "say_hello worked" << std::endl;
		return 0;
	}

    const char TileMap::className[] = "TileMap";


	const Luna<TileMap>::PropertyType TileMap::properties[] = {
		{0,0}
	};


	const Luna<TileMap>::FunctionType TileMap::methods[] = {
		method(TileMap, say_hello, lua_say_hello),
		{0,0}
	};
}

#endif