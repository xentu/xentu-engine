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
		m_map = new tmx::Map();
		m_layers_count = 0;
        Advisor::logInfo("Created instance of TileMap.");
	}


    TileMap::~TileMap()
	{
		for (int i = 0; i < m_layers_count; i++)
			delete m_layers[i];

		if (m_map != nullptr) {
			delete m_map;
		}
        Advisor::logInfo("Deleted instance of TileMap.");
    }


	void TileMap::load(lua_State* L, std::string filename)
	{
		if (m_map->load(filename))
		{
			// TODO: Load textures, and process layers so that drawing is made easier.

			const auto& layers = m_map->getLayers();
			std::cout << "Map has " << layers.size() << " layers" <<  std::endl;
			m_layers_count = 0;

			for (const auto& layer : layers)
			{
				const tmx::Layer::Ptr& l = layer;
				m_layers[m_layers_count] = new TileMapLayer(L, l);
				m_layers_count++;				
				// std::cout << "Found Layer: " << layer->getName() << std::endl;
				// std::cout << "Layer Type: " << int(layer->getType()) << std::endl;
			}
			xen::Advisor::logInfo("Loaded tmx!");
		}
		else {
			xen::Advisor::logInfo("Failed to load tmx :(");
		}
	}


	const tmx::Map* TileMap::get_map() const
	{
		return m_map;
	}


	int TileMap::lua_get_layers_count(lua_State* L)
	{
		lua_pushinteger(L, m_layers_count);
		return 1;
	}


	int TileMap::lua_get_layer(lua_State* L)
	{
		int layer_index = lua_tointeger(L, -1);
		int max_index = m_layers_count - 1;

		if (layer_index < 0 || layer_index > max_index) {
			Advisor::throwError("Tried to access a TileLayer with an invalid index.");
			return 0;
		}
		else {
			// send it to lua.
			Luna<TileMapLayer>::push(L, m_layers[layer_index]);
		}
	}


    const char TileMap::className[] = "TileMap";


	const Luna<TileMap>::PropertyType TileMap::properties[] = {
		{0,0}
	};


	const Luna<TileMap>::FunctionType TileMap::methods[] = {
		method(TileMap, get_layers_count, lua_get_layers_count),
		method(TileMap, get_layer, lua_get_layer),
		{0,0}
	};
}

#endif