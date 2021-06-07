#ifndef XEN_LUA_SPRITE
#define XEN_LUA_SPRITE

#include <string>

namespace xen
{
	/** A type used to describe info sent from lua */
	struct LuaSprite {
		int texture;
		int x;
		int y;
		int width;
		int height;
		int spritemap;
		std::string region;
	};
}

#endif