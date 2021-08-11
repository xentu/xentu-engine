#ifndef XEN_SPRITE_MAP_CPP
#define XEN_SPRITE_MAP_CPP

#include <iostream>
#include <sstream>
#include <fstream>

#include <json/json.hpp>
#include "SpriteMap.h"
#include "../utilities/Advisor.h"

namespace xen
{
	SpriteMap::SpriteMap()
	{
		
	}


	SpriteMap::~SpriteMap()
	{

	}


	SpriteMap* SpriteMap::parse_file(std::string filename)
	{
		SpriteMap* result = new SpriteMap();

		/* try to read the specified file. */
		std::ifstream ifs(filename);
		nlohmann::json j;
		ifs >> j;

		for (auto& el : j.items()) {
			auto en = el.value();
			if (en.contains("x") && en.contains("y") && en.contains("width") && en.contains("height")) {
				float x = en["x"];
				float y = en["y"];
				float w = en["width"];
				float h = en["height"];
				result->add_region(el.key(), new Rect(x, y, w, h));
			}
			else {
				Advisor::logInfo("Ignored SpriteMap entry: ", el.key(), " : ", el.value());
			}
		}

		return result;
	}


	const Rect* SpriteMap::get_region(const std::string nickname) const
	{
		if (regions.count(nickname))
		{
			return regions.at(nickname);
		}
		return nullptr;
	}


	int SpriteMap::add_region(const std::string nickname, const Rect* rect)
	{
		if (regions.count(nickname))
		{
			return -1;
		}

		const auto pair = std::make_pair(nickname, rect);
		regions.insert(pair);
		return 0;
	}
}

#endif