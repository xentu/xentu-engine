#pragma once

#include <map>
#include <string>

#include <lib/tmxlite/Map.hpp>
#include "Tile.h"
#include "Vector2.h"

namespace xen
{
	constexpr unsigned int MAX_POLYGON_POINTS = 1024;

	class TileMapObject
	{
	public:
		TileMapObject();
		TileMapObject(const tmx::Object object);
		~TileMapObject(void);

		const xen::Tile GetTile() const;

		bool has_tile;
		float x;
		float y;
		float width;
		float height;
		xen::Tile m_tile;

	private:
		const tmx::Object m_object;

		// polygon points.
		xen::Vector2f m_points[MAX_POLYGON_POINTS];
		unsigned int m_point_count;
	};
}