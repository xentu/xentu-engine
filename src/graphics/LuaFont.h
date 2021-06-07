#ifndef XEN_LUA_FONT
#define XEN_LUA_FONT

namespace xen
{
	/** A type used to describe info sent from lua */
	struct LuaFont {
		int texture_id;
		int spritemap_id;
        int letter_spacing;
        int line_height;
	};
}

#endif