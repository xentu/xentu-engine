#include <iostream>
#include <sstream>
#include <fstream>

#include <json/json.hpp>
#include "SpriteMap.h"
#include "../assets/AssetManager.h"
#include "../vfs/XenVirtualFileSystem.h"
#include "../vfs/XenFileSystem.h"

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

		std::string image = j.at("image");
		auto a = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(image);
		int texture_id = a->LoadTexture(res.buffer, res.length);
		auto texture = a->GetTexture(texture_id);
		result->set_texture(texture_id);

		float texture_w = static_cast<float>(texture->width);
		float texture_h = static_cast<float>(texture->height);

		auto animations = j.at("animations");

		for (auto& el : animations.items())
		{
			printf("animation!\n");
			auto en = el.value();
			if (en.contains("name") && en.contains("frames"))
			{
				const std::string name = en["name"];
				SpriteMapGroup* group = new SpriteMapGroup();
				SpriteMapFrame* frame = new SpriteMapFrame();
				frame->coords = new Rect(0, 0, 0.1655, 0.1420);
				group->add_frame(frame);
				result->add_group(name, group);

				//const std::string coords = en["frames"];
				printf("animation '%s'\n", name.c_str());
			}
		}

		printf("image2: %s\n", image.c_str());

		/* for (auto& el : j.items()) {
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
		} */

		return result;
	}


	const SpriteMapGroup* SpriteMap::get_group(const std::string nickname) const
	{
		if (m_groups.count(nickname))
		{
			return m_groups.at(nickname);
		}
		return nullptr;
	}


	int SpriteMap::add_group(const std::string nickname, const SpriteMapGroup* group)
	{
		if (m_groups.count(nickname))
		{
			return -1;
		}

		const auto pair = std::make_pair(nickname, group);
		m_groups.insert(pair);
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
		m_groups.clear();
	}
}