#ifndef XEN_LUA_COLOR
#define XEN_LUA_COLOR

namespace xen
{
	/** A type used to describe info sent from lua */
	struct LuaColor {
		float red;
		float green;
		float blue;
		float alpha;

		static LuaColor White()
		{
			LuaColor c;
			c.red = 1;
			c.green = 1;
			c.blue = 1;
			c.alpha = 1;
			return c;
		}
	};
}

#endif