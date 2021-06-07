#ifndef XEN_SPRITE_MAP_CPP
#define XEN_SPRITE_MAP_CPP

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
				std::cout << "Ignored SpriteMap entry: " << el.key() << " : " << el.value() << "\n";
			}
		}

		return result;
	}


	Rect* SpriteMap::get_region(std::string nickname)
	{
		if (regions.count(nickname))
		{
			return regions[nickname];
		}
		return nullptr;
	}


	int SpriteMap::add_region(std::string nickname, Rect* rect)
	{
		if (regions.count(nickname))
		{
			return -1;
		}

		regions.insert(std::make_pair(nickname, rect));
		return 0;
	}
}

#endif