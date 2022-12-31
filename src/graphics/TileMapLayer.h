#pragma once

#include <map>
#include <string>

#include <lib/tmxlite/Map.hpp>
#include <lib/tmxlite/TileLayer.hpp>
#include <lib/tmxlite/ImageLayer.hpp>

#include "Tile.h"
#include "TileMapObject.h"


namespace xen
{
	constexpr int MAX_OBJECTS_OR_TILES = 65536;


	class TileMapLayer
	{
	public:
		TileMapLayer();
		TileMapLayer(tmx::Map& map, const tmx::Layer::Ptr& layer);
		~TileMapLayer(void);

		const tmx::Vector2u get_size() const;
		std::vector<tmx::TileLayer::Tile> get_tiles(const tmx::Layer::Ptr& layer) const;
		const int get_texture_id() const;
		const int get_object_count() const;
		const TileMapObject* get_object(const int object_index);

		Tile *m_tiles[MAX_OBJECTS_OR_TILES];
		int m_tile_count;
	private:
		// properties from the old m_layer
		tmx::Vector2u m_size;
		tmx::Vector2i m_offset;
		tmx::Layer::Type m_type;


		// object info.
		TileMapObject *m_objects[MAX_OBJECTS_OR_TILES];
		int m_object_count;

		// background texture.
		int m_texture_id;
		std::string m_name;
		float m_opacity;
		bool m_visible;
	};
}