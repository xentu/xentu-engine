#pragma once

#define NOMINMAX

#include "../Globals.h"
#include "../assets/AssetManager.h"
#include "TileMapLayer.h"
#include "Tile.h"

namespace xen {
	
	TileMapLayer::TileMapLayer()
	    : m_size(tmx::Vector2u(0, 0)),
		  m_offset(tmx::Vector2i(0, 0)),
		  m_type(tmx::Layer::Type::Object),
		  m_tile_count(0),
		  m_object_count(0),
		  m_objects{},
		  m_tiles{},
		  m_texture_id(0),
		  m_name("Untitled"),
		  m_opacity(1.0f),
		  m_visible(true)
	{ }


	TileMapLayer::TileMapLayer(tmx::Map& map, const tmx::Layer::Ptr& layer) 
		: m_size(layer->getSize()),
		  m_offset(layer->getOffset()),
		  m_type(layer->getType()),
		  m_tile_count(0),
		  m_object_count(0),
		  m_objects{},
		  m_tiles{},
		  m_texture_id(0),
		  m_name(layer->getName()),
		  m_opacity(layer->getOpacity()),
		  m_visible(layer->getVisible())
	{
		auto assets = AssetManager::GetInstance();
		tmx::Layer::Type type = layer->getType();


		const auto& tileSize = map.getTileSize();
		const auto& tileSets = map.getTilesets();
		auto tileSetTextures = new std::map<std::string, int>();
		const auto layerSize = this->GetSize();
		std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
		std::vector<const tmx::Tileset*> usedTileSets;


		if (type == tmx::Layer::Type::Object)
		{
			auto tileSetTextures = new std::map<std::string, int>();
			const auto object_group = layer->getLayerAs<tmx::ObjectGroup>();
			const auto objects = object_group.getObjects();
			for (const tmx::Object object : objects)
			{
				auto tobj = new TileMapObject(object);

				// identify which tile sets are used by the object.
				uint32_t idx = object.getTileID();
				if (idx > 0) {
					bool found = false;
					int index = 0;
					for (auto ts = tileSets.rbegin(); ts != tileSets.rend(); ++ts)
					{
						index++;
						if (idx >= ts->getFirstGID() && idx < maxID)
						{
							const auto& path = ts->getImagePath();
							auto vec2 = &object.getPosition();
							int texture_id = assets->LoadTexture(path);
							auto ts_tile = ts->getTile(idx);
							auto ts_tile_size = ts->getTileSize();

							tobj->m_tile = Tile(
								static_cast<int>(vec2->x),
								static_cast<int>(vec2->y) - ts_tile_size.y,
								ts_tile_size.x,
								ts_tile_size.y,
								texture_id);
							tobj->m_tile.t_x = ts_tile->imagePosition.x;
							tobj->m_tile.t_y = ts_tile->imagePosition.y;
							tobj->m_tile.t_width = ts_tile_size.x;
							tobj->m_tile.t_height = ts_tile_size.y;
							tobj->has_tile = true;
							tobj->y = tobj->y - ts_tile_size.y;
							found = true;
							break;
						}
					}

					if (found == false) {
						XEN_WARN("Failed to find tile set.");
					}
				}

				//if (tobj->has_tile) {
					m_objects[m_object_count] = tobj;
					m_object_count++;
				//}
			}
		}
		else if (type == tmx::Layer::Type::Image)
		{
			const auto image = layer->getLayerAs<tmx::ImageLayer>();
			const auto image_path = image.getImagePath();
			m_texture_id = assets->LoadTexture(image_path); // TX_RGBA - TX_CLAMP_TO_EDGE
		}
		else if (type == tmx::Layer::Type::Tile)
		{
			const auto& tiles = this->GetTiles(layer);
			
			unsigned int idx = 0;

			// identify which tilesets are used by the layer.
			for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
			{
				for (const auto& tile : tiles)
				{
					if (tile.ID >= i->getFirstGID() && tile.ID < maxID)
					{
						usedTileSets.push_back(&(*i));
						break;
					}
				}
				//maxID = i->getFirstGID();
			}

			// make sure tileset textures are loaded.
			for (const auto& ts : usedTileSets)
			{
				const auto& path = ts->getImagePath();
				const int texture_id = assets->LoadTexture(path); // TX_RGBA - TX_CLAMP_TO_EDGE
				tileSetTextures->insert(std::make_pair(ts->getName(), texture_id));
			}

			// create the tiles one by one.
			for (unsigned int row = 0; row < layerSize.y; ++row)
			{
				for (unsigned int col = 0; col < layerSize.x; ++col)
				{
					const uint32_t global_id = tiles[idx].ID; // the global id of the tile to use.
					Tile* new_tile = new Tile(col * tileSize.x, row * tileSize.y, tileSize.x, tileSize.y);

					// find the tileset, to give us the the subrect to add to new_tile for texture.
					for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
					{
						if (global_id >= i->getFirstGID() && global_id <= i->getLastGID())
						{
							const auto ts_tile = i->getTile(global_id);
							const auto ts_tile_size = i->getTileSize();
							new_tile->t_x = ts_tile->imagePosition.x;
							new_tile->t_y = ts_tile->imagePosition.y;
							new_tile->t_width = ts_tile_size.x;
							new_tile->t_height = ts_tile_size.y;
							new_tile->texture_id = tileSetTextures->at(i->getName());
							break;
						}
					}

					m_tiles[m_tile_count] = new_tile;
					m_tile_count++;
					idx++;
				}
			}
		
			// be a good coder and clean up your mess!
			delete tileSetTextures;
		}

		XEN_ECHO("Loaded instance of TileMapLayer (%s).\n", layer->getName().c_str());
	}

	
	TileMapLayer::~TileMapLayer(void)
	{
		if (m_tile_count > 0)
		{
			for (int i = 0; i < m_tile_count; i++)
			{
				delete m_tiles[i];
			}
		}
		
		if (m_object_count > 0)
		{
			for (int i = 0; i < m_object_count; i++)
			{
				delete m_objects[i];
			}
		}

		//Advisor::logInfo("Deleted instance of TileMapLayer.");
	}


	const tmx::Vector2u TileMapLayer::GetSize() const
	{
		return m_size;
	}


	std::vector<tmx::TileLayer::Tile> TileMapLayer::GetTiles(const tmx::Layer::Ptr& layer) const
	{
		tmx::TileLayer& tile_layer = layer->getLayerAs<tmx::TileLayer>();
		return tile_layer.getTiles();
	}


	const int TileMapLayer::GetTextureID() const
	{
		return m_texture_id;
	}


	const int TileMapLayer::GetObjectCount() const
	{
		return m_object_count;
	}

	
	const TileMapObject* TileMapLayer::GetObject(const int object_index)
	{
		int max_index = m_object_count - 1;

		if (object_index < 0 || object_index > max_index) {
			//Advisor::throwError("Tried to access a TileMapObject with an invalid index.");
			return 0;
		}
		
		// send it to lua.
		return m_objects[object_index];
	}
}