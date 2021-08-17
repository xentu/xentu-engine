#ifndef XEN_TILEMAP_LAYER_CPP
#define XEN_TILEMAP_LAYER_CPP

#define NOMINMAX

#include "XentuGame.h"
#include "TileMapLayer.h"
#include "../utilities/Advisor.h"

// Specify a macro for storing information about a class and method name, this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

namespace xen {
	
	TileMapLayer::TileMapLayer(lua_State* L)
		: m_layer(nullptr),
		  m_tile_count(0),
		  m_object_count(0),
		  m_objects{},
		  m_tiles{}
	{ }


	TileMapLayer::TileMapLayer(lua_State* L, tmx::Map& map, const tmx::Layer::Ptr& layer) 
		: m_layer(layer),
		  m_tile_count(0),
		  m_object_count(0),
		  m_objects{},
		  m_tiles{}
	{
		tmx::Layer::Type type = layer->getType();

		if (type == tmx::Layer::Type::Object)
		{
			auto object_group = layer->getLayerAs<tmx::ObjectGroup>();
			auto objects = object_group.getObjects();
			for (const tmx::Object object : objects)
			{
				m_objects[m_object_count] = new TileMapObject(L, object);
				m_object_count++;
			}
		}
		else if (type == tmx::Layer::Type::Tile)
		{
			XentuGame* game = XentuGame::get_instance(L);
			const auto& tiles = this->get_tiles();
			const auto& tileSize = map.getTileSize();
			const auto& tileSets = map.getTilesets();
			auto tileSetTextures = new std::map<std::string, int>();
			const auto layerSize = this->get_size();
			std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
			std::vector<const tmx::Tileset*> usedTileSets;
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
				maxID = i->getFirstGID();
			}

			// make sure tileset textures are loaded.
			for (const auto& ts : usedTileSets)
			{
				const auto& path = ts->getImagePath();
				int texture_id = game->assets->load_texture(path, 2, 1); // TX_RGBA - TX_CLAMP_TO_EDGE
				tileSetTextures->insert(std::make_pair(ts->getName(), texture_id));
			}

			// create the tiles one by one.
			for (auto row = 0; row < layerSize.y; ++row)
			{
				for (auto col = 0; col < layerSize.x; ++col)
				{
					uint32_t global_id = tiles[idx].ID; // the global id of the tile to use.
					Tile* new_tile = new Tile(col * tileSize.x, row * tileSize.y, tileSize.x, tileSize.y);

					// find the tileset, to give us the the subrect to add to new_tile for texture.
					for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
					{
						if (global_id >= i->getFirstGID() && global_id <= i->getLastGID())
						{
							auto ts_tile = i->getTile(global_id);
							auto ts_tile_size = i->getTileSize();
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

		Advisor::logInfo("Loaded instance of TileMapLayer.");
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

		Advisor::logInfo("Deleted instance of TileMapLayer.");
	}


	const tmx::Layer::Ptr& TileMapLayer::get_layer() const
	{
		return m_layer;
	}


	const tmx::Vector2u TileMapLayer::get_size() const
	{
		return m_layer->getSize();
	}


	int TileMapLayer::lua_get_name(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushstring(L, "james");
		}
		else {
			lua_pushstring(L, m_layer->getName().c_str());
		}
		return 1;
	}


	int TileMapLayer::lua_get_offset(lua_State* L) {
		if (m_layer == nullptr) {
			auto vec2 = m_layer->getOffset();
			lua_pushinteger(L, 0);
			lua_pushinteger(L, 0);
		}
		else {
			auto vec2 = &m_layer->getOffset();
			lua_pushinteger(L, vec2->x);
			lua_pushinteger(L, vec2->y);
		}
		return 2;
	}


	int TileMapLayer::lua_get_opacity(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushnumber(L, 1.0f);
		}
		else {
			lua_pushnumber(L, m_layer->getOpacity());
		}
		return 1;
	}


	int TileMapLayer::lua_get_size(lua_State* L) {
		if (m_layer == nullptr) {
			auto vec2 = m_layer->getSize();
			lua_pushinteger(L, 0);
			lua_pushinteger(L, 0);
		}
		else {
			auto vec2 = m_layer->getSize();
			lua_pushinteger(L, vec2.x);
			lua_pushinteger(L, vec2.y);
		}
		return 2;
	}


	int TileMapLayer::lua_get_type(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushstring(L, "");
		}
		else {
			switch (m_layer->getType())
			{
				case tmx::Layer::Type::Group:
					lua_pushstring(L, "Group");
					break;
				case tmx::Layer::Type::Image:
					lua_pushstring(L, "Image");
					break;
				case tmx::Layer::Type::Object:
					lua_pushstring(L, "Object");
					break;
				case tmx::Layer::Type::Tile:
					lua_pushstring(L, "Tile");
					break;
				default:
					lua_pushstring(L, "Unknown");
					break;
			}
		}
		return 1;
	}


	int TileMapLayer::lua_get_visible(lua_State* L) {
		if (m_layer == nullptr) {
			lua_pushboolean(L, false);
		}
		else {
			lua_pushboolean(L, m_layer->getVisible());
		}
		return 1;
	}


	int TileMapLayer::lua_get_objects_count(lua_State* L)
	{
		lua_pushinteger(L, m_object_count);
		return 1;
	}


	int TileMapLayer::lua_get_object(lua_State* L)
	{
		int object_index = lua_tointeger(L, -1);
		int max_index = m_object_count - 1;

		if (object_index < 0 || object_index > max_index) {
			Advisor::throwError("Tried to access a TileLayer with an invalid index.");
			return 0;
		}
		else {
			// send it to lua.
			Luna<TileMapObject>::push(L, m_objects[object_index]);
		}
	}


	std::vector<tmx::TileLayer::Tile> TileMapLayer::get_tiles() const
	{
		tmx::TileLayer& tile_layer = m_layer->getLayerAs<tmx::TileLayer>();
		return tile_layer.getTiles();
	}


	const char TileMapLayer::className[] = "TileMapLayer";


	const Luna<TileMapLayer>::PropertyType TileMapLayer::properties[] = {
		{0,0}
	};


	const Luna<TileMapLayer>::FunctionType TileMapLayer::methods[] = {
		method(TileMapLayer, get_name, lua_get_name),
		method(TileMapLayer, get_offset, lua_get_offset),
		method(TileMapLayer, get_opacity, lua_get_opacity),
		method(TileMapLayer, get_size, lua_get_size),
		method(TileMapLayer, get_type, lua_get_type),
		method(TileMapLayer, get_visible, lua_get_visible),
		method(TileMapLayer, get_objects_count, lua_get_objects_count),
		method(TileMapLayer, get_object, lua_get_object),
		{0,0}
	};
}


#endif