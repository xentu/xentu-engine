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

		// get the texture size so we can convert pixel coords to uv's
		float texture_w = static_cast<float>(texture->width);
		float texture_h = static_cast<float>(texture->height);

		// avoid division by 0
		if (texture_w == 0) texture_w = 1;
		if (texture_h == 0) texture_h = 1;

		auto animations = j.at("animations");

		for (auto& el : animations.items())
		{
			auto en = el.value();
			if (en.contains("name") && en.contains("frames"))
			{
				const std::string name = en["name"];
				SpriteMapGroup* group = new SpriteMapGroup();

				for (auto& el_frame : en.at("frames").items())
				{
					auto en_frame_val = el_frame.value();
					SpriteMapFrame* frame = new SpriteMapFrame();

					// convert coords to integers.
					string str_coords = en_frame_val["coords"];
					string raw_coords;
					std::remove_copy(str_coords.begin(), str_coords.end(), std::back_inserter(raw_coords), ',');
					
					// convert the coords string into a set of floats.
					std::vector<float> parsed_coords;
					std::istringstream iss(raw_coords);
					std::copy(std::istream_iterator<float>(iss), std::istream_iterator<float>(), std::back_inserter(parsed_coords));
					
					// get the pixel coords from the parsed string.
					Rect px_coords = Rect(parsed_coords[0], parsed_coords[1], parsed_coords[2], parsed_coords[3]);

					// convert the pixel coords into uv's to help the renderer.
					frame->coords = new Rect(
						px_coords.left / texture_w, // u
						px_coords.top / texture_h, // v
						px_coords.width / texture_w, // w
						px_coords.height / texture_h  // h
					);

					// add the frame to the group.
					group->add_frame(frame);
				}
				
				result->add_group(name, group);
				printf("-- animation '%s --'\n", name.c_str());
			}
		}

		printf("image2: %s\n", image.c_str());

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