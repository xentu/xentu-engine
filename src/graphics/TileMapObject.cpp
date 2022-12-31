#pragma once

#include "TileMapObject.h"
#include "Vector2.h"

namespace xen
{
	TileMapObject::TileMapObject() :
		has_tile(false)
	{ }


	TileMapObject::TileMapObject(tmx::Object object) :
		has_tile(false),
		m_object(object)
	{
		const auto aabb = &m_object.getAABB();
		x = aabb->left;
		y = aabb->top;
		width = aabb->width;
		height = aabb->height;
		m_point_count = 0;

		const auto points = object.getPoints();
		if (points.size() > 0) {
			for (const tmx::Vector2f point : points)
			{
				m_points[m_point_count] = Vector2f(point.x, point.y);
				m_point_count++;
			}
		}
	}


	TileMapObject::~TileMapObject(void)
	{

	}


	const xen::Tile TileMapObject::GetTile() const
	{
		return m_tile;
	}
}