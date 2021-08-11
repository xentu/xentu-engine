#ifndef XEN_SPRITE_MAP_HPP
#define XEN_SPRITE_MAP_HPP

#include <map>
#include <string>

#include "Rect.h"

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

		static SpriteMap* parse_file(std::string filename);

		/// <summary>
		/// Attempt to get a named region, if a region is not found, a default one is returned instead.
		/// </summary>
		/// <param name="nickname">The name of the region to return.</param>
		/// <returns>The rectangle that represents the found region.</returns>
		const Rect* get_region(std::string nickname) const;
		int add_region(const std::string nickname, const Rect* rect);
	private:

		std::map<const std::string, const Rect*> regions;
	};
}

#endif