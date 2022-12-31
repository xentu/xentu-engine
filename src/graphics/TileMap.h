#pragma once

#include <map>
#include <string>

#include <lib/tmxlite/Map.hpp>
#include "TileMapLayer.h"

namespace xen
{
	class TileMap
	{
	public:
		TileMap();
		~TileMap(void);

		void LoadTMX(const std::string& xml, const std::string& working_dir);

		//const tmx::Map* get_map() const;
		
		int GetLayerCount();

	private:
		//tmx::Map* m_map;
		TileMapLayer *m_layers[100]; // max 100 layers.
		int m_layers_count;
	};
}