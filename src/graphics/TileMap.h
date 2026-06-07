#ifndef TILEMAP_H
#define TILEMAP_H

#include <map>
#include <string>

#include <tmxlite/Map.hpp>
#include "TileMapLayer.h"

namespace xen
{
	class TileMap
	{
	public:
		TileMap();
		~TileMap(void);

		void LoadTMX(const std::string& xml, const std::string& working_dir);
		
		int GetLayerCount();

		TileMapLayer* GetLayer(int layer_index);

	private:
		//tmx::Map* m_map;
		TileMapLayer *m_layers[100]; // max 100 layers.
		int m_layers_count;
	};
}

#endif //TILEMAP_H