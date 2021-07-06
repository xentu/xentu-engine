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

		static LuaFont Default()
		{
			LuaFont f;
			f.texture_id = -1;
			f.spritemap_id = -1;
			f.letter_spacing = 1;
			f.line_height = 12;
			return f;
		}
	};
}

#endif