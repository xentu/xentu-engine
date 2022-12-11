#pragma once

#include <map>
#include <string>

#include "Rect.h"
#include "SpriteMapGroup.h"

namespace xen
{
	class SpriteMap
	{
	public:
		/// <summary>
		/// Create a new asset manager instance.
		/// </summary>
		SpriteMap();
		~SpriteMap();

		static SpriteMap* parse_file(std::string const& json);

		/// <summary>
		/// Attempt to get a named region, if a region is not found, a default one is returned instead.
		/// </summary>
		/// <param name="nickname">The name of the region to return.</param>
		/// <returns>The rectangle that represents the found region.</returns>
		const SpriteMapGroup* get_group(std::string nickname) const;
		int add_group(const std::string nickname, const SpriteMapGroup* group);

		void set_texture(int texture_asset_id);
		const int get_texture() const;


	private:
		// the asset_id for the loaded texture.
		int m_texture_asset_id;
		std::map<const std::string, const SpriteMapGroup*> m_groups;
	};
}