#include <iostream>
#include <limits>

#include "../Globals.h"
#include "TileMap.h"

namespace xen
{
	TileMap::TileMap()
	{
		//m_map = new tmx::Map();
		m_layers_count = 0;
      //Advisor::logInfo("Created instance of TileMap.");
	}


	TileMap::~TileMap()
	{
		for (int i = 0; i < m_layers_count; i++)
			delete m_layers[i];

		/* if (m_map != nullptr) {
			delete m_map;
		} */
	}


	void TileMap::LoadTMX(const std::string& xml, const std::string& working_dir)
	{
		tmx::Map* map = new tmx::Map;
		if (map->loadFromString(xml, working_dir))
		{
			// TODO: Load textures, and process layers so that drawing is made easier.

			const auto& layers = map->getLayers();
			XEN_ECHO("Map has %d layers\n", layers.size());
			m_layers_count = 0;

			for (const tmx::Layer::Ptr& layer : layers)
			{
				const tmx::Layer::Ptr& l = layer;
				m_layers[m_layers_count] = new TileMapLayer(*map, l);
				m_layers_count++;
			}
			//xen::Advisor::logInfo("Loaded tmx!");
		}
		else {
			//xen::Advisor::logInfo("Failed to load tmx :(");
		}
		delete map;
	}


	int TileMap::GetLayerCount()
	{
		return m_layers_count;
	}


	TileMapLayer* TileMap::GetLayer(int layer_index)
	{
		return m_layers[layer_index];
	}
}