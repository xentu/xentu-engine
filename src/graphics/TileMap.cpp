#include <iostream>
#include <limits>

#include "../Globals.h"
#include "TileMap.h"

namespace xen
{
	TileMap::TileMap(lua_State* L)
	{
		m_map = new tmx::Map();
		m_layers_count = 0;
      //Advisor::logInfo("Created instance of TileMap.");
	}


	TileMap::~TileMap()
	{
		for (int i = 0; i < m_layers_count; i++)
			delete m_layers[i];

		if (m_map != nullptr) {
			delete m_map;
		}
		//Advisor::logInfo("Deleted instance of TileMap.");
	}


	void TileMap::load(std::string filename)
	{
		if (m_map->load(filename))
		{
			// TODO: Load textures, and process layers so that drawing is made easier.

			const auto& layers = m_map->getLayers();
			XEN_ECHO("Map has ", std::to_string( layers.size() ), " layers");
			m_layers_count = 0;

			for (const tmx::Layer::Ptr& layer : layers)
			{
				const tmx::Layer::Ptr& l = layer;
				m_layers[m_layers_count] = new TileMapLayer(*m_map, l);
				m_layers_count++;
			}
			//xen::Advisor::logInfo("Loaded tmx!");
		}
		else {
			//xen::Advisor::logInfo("Failed to load tmx :(");
		}
	}


	const tmx::Map* TileMap::get_map() const
	{
		return m_map;
	}


	int TileMap::get_layers_count()
	{
		return m_layers_count;
	}
}