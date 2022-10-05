#include <iostream>
#include <sstream>
#include <fstream>

#include <json/json.hpp>
#include "SpriteMap.h"

namespace xen
{
	SpriteMap::SpriteMap()
	{
		
	}


	SpriteMap::~SpriteMap()
	{
		// TODO: memory leak, delete the rects!
		m_texture_asset_id = -1;
	}


	SpriteMap* SpriteMap::parse_file(std::string const& json)
	{
		SpriteMap* result = new SpriteMap();
		nlohmann::json j = nlohmann::json::parse(json);

		for (auto& el : j.items()) {
			auto en = el.value();
			if (en.contains("x") && en.contains("y") && en.contains("width") && en.contains("height")) {
				const float w = en["width"];
				const float h = en["height"];
				const float x = en["x"];
				const float raw_y = en["y"];
				const float y = 1.0f - raw_y - h;
				const auto reg = el.key();
				result->add_region(reg, new Rect(x, y, w, h));
			}
			else {
				//Advisor::logInfo("Ignored SpriteMap entry: ", el.key(), " : ", el.value());
			}
		}

		return result;
	}


	const Rect* SpriteMap::get_region(const std::string nickname) const
	{
		if (m_regions.count(nickname))
		{
			return m_regions.at(nickname);
		}
		return nullptr;
	}


	int SpriteMap::add_region(const std::string nickname, const Rect* rect)
	{
		if (m_regions.count(nickname))
		{
			return -1;
		}

		const auto pair = std::make_pair(nickname, rect);
		m_regions.insert(pair);
		return 0;
	}


	void SpriteMap::set_texture(int texture_asset_id) {
		m_texture_asset_id = texture_asset_id;
	}


	const int SpriteMap::get_texture() const {
		return m_texture_asset_id;
	}


	void SpriteMap::reset() {
		// TODO: clean up memory, rects are on heap.
		m_regions.clear();
	}
}