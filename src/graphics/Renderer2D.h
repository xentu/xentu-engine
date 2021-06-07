#ifndef XEN_SPRITE_BATCH_HPP
#define XEN_SPRITE_BATCH_HPP

// see: https://github.com/ricanteja/Moony-Renderer2D/blob/master/source/Renderer2D.h

#include <array>
#include <iostream>
#include <random>
#include <vector>

#include <luna/luna.hpp>
#include "LuaFont.h"
#include "LuaSprite.h"
#include "Batch.h"

namespace xen
{
	/// <summary>
	/// The main rendering engine for Xentu, provides methods for rendering graphics
	/// similar to a SpriteBatch in other engines.
	/// </summary>
	class Renderer2D
	{
	public:
		Renderer2D(lua_State* L);
		~Renderer2D();

		/// <summary>
		/// Call this before the Renderer2D is used, but after OpenGL context is created.
		/// </summary>
		void initialize();

		/// <summary>
		/// Begins the sequence for rendering sprites.
		/// </summary>
		int begin(lua_State* L);

		/// <summary>
		/// Call this to flush the buffer and draw the loaded sprites.
		/// </summary>
		int present(lua_State* L);

		/// <summary>
		/// Draw a sprite from Lua.
		/// </summary>
		int draw_sprite(lua_State* L);

		/// <summary>
		/// Draw sone text from Lua
		/// </summary>
		int draw_text(lua_State* L);

		/// <summary>
		/// 
		/// </summary>
		int set_rotation(lua_State* L);

		/// <summary>
		/// 
		/// </summary>
		int set_origin(lua_State* L);

		/// <summary>
		/// 
		/// </summary>
		int set_scale(lua_State* L);

		/// <summary>
		/// Clear the sprite batch buffer.
		/// </summary>
		void clear();

		/// <summary>
		/// Draw a sprite.
		/// </summary>
		/// <param name="sprite"></param>
		void draw(const Sprite& sprite);

		/// <summary>
		/// Sort the batch.
		/// </summary>
		void order();

		/// <summary>
		/// Called by lua to ensure a sprite object is formatted correctly.
		/// </summary>
		int debug_sprite(lua_State* L);

		//Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<Renderer2D>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<Renderer2D>::FunctionType methods[];

	private:
		Batch* find_batch(const Sprite& sprite);
		LuaSprite parse_lua_sprite(lua_State* L);
		LuaFont parse_lua_font(lua_State* L);

		bool m_initialized;
		std::vector<Batch*> m_batches;
		Sprite m_sprite;
		float m_rotation;
		float m_origin_x;
		float m_origin_y;
		float m_scale_x;
		float m_scale_y;

		/* the vertex/index buffer. */
		unsigned int vbo;
		unsigned int ibo;
	};
}

#endif