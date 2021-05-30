#ifndef XEN_VIEWPORT_HPP
#define XEN_VIEWPORT_HPP


#include <luna/luna.hpp>


namespace xen
{
	struct Viewport
	{
    public:
		Viewport(lua_State* L);
		Viewport(int width, int height);
        ~Viewport(void);

        int get_width(lua_State* L);
        int get_height(lua_State* L);

        //Class Constants
        static const char className[];

        // List of class properties that one can set/get from Lua
        static const Luna<Viewport>::PropertyType properties[];

        // List of class methods to make available in Lua
        static const Luna<Viewport>::FunctionType methods[];

		int width;
		int height;
	};
}

#endif