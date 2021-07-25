#ifndef XEN_RENDERER2D_HPP
#define XEN_RENDERER2D_HPP
#define GLFW_INCLUDE_NONE

#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <luna/luna.hpp>

#include "lua/LuaFont.h"
#include "lua/LuaSprite.h"
#include "lua/LuaColor.h"
#include "Batch.h"
#include "Texture.h"


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


#pragma region Public Methods


		/// <summary>
		/// Call this before the Renderer2D is used, but after OpenGL init.
		/// </summary>
		void initialize(lua_State* L);

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
		/// Clear the sprite batch buffer.
		/// </summary>
		void clear();


#pragma endregion


#pragma region Lua Accessible Methods


		/// <summary>
		/// Begins the sequence for rendering sprites.
		/// </summary>
		int lua_begin(lua_State* L);
		
		/// <summary>
		/// Called by lua to ensure a sprite object is formatted correctly.
		/// </summary>
		int lua_debug_sprite(lua_State* L);

		/// <summary>
		/// Draw a sprite from Lua.
		/// </summary>
		int lua_draw_sprite(lua_State* L);

		/// <summary>
		/// Draw a rectangle from Lua.
		/// </summary>
		int lua_draw_rect(lua_State* L);

		/// <summary>
		/// Draw sone text from Lua.
		/// </summary>
		int lua_draw_text(lua_State* L);

		/// <summary>
		/// Draw a tilemap from Lua.
		/// </summary>
		int lua_draw_tilemap(lua_State* L);

		/// <summary>
		/// Call this to flush the buffer and draw the loaded sprites.
		/// </summary>
		int lua_present(lua_State* L);

		/// <summary>
		/// Enable or disable blending.
		/// </summary>
		int lua_set_blend(lua_State* L);

		/// <summary>
		/// Set the blend state for the renderer.
		/// </summary>
		int lua_set_blend_func(lua_State* L);

		/// <summary>
		/// Set the color used to clear the screen.
		/// </summary>
		int lua_set_clear_color(lua_State* L);

		/// <summary>
		/// Set the x,y transformational origin of the next drawn sprite, default is 0,0.
		/// </summary>
		int lua_set_origin(lua_State* L);

		/// <summary>
		/// Set the rotation angle of the next drawn sprite, default is 0
		/// </summary>
		int lua_set_rotation(lua_State* L);

		/// <summary>
		/// Set the scale of the next drawn sprite, default is 1,1.
		/// </summary>
		int lua_set_scale(lua_State* L);

		/// <summary>
		/// Assign a shader to be used. If nil is passed, the default shader is used instead.
		/// </summary>
		int lua_set_shader(lua_State* L);		


#pragma endregion


		// Class Constants
		static const char className[];

		// List of class properties that one can set/get from Lua
		static const Luna<Renderer2D>::PropertyType properties[];

		// List of class methods to make available in Lua
		static const Luna<Renderer2D>::FunctionType methods[];

	private:
		// Find the correct batch to use for rendering a sprite.
		Batch* find_batch(const Sprite& sprite);
		LuaSprite parse_lua_sprite(lua_State* L);
		LuaFont parse_lua_font(lua_State* L);
		LuaColor parse_lua_color(lua_State* L);
		Vector4f parse_color(lua_State* L);

		/* state information */
		bool m_initialized; // weather or not the renderer has been initialized.
		std::vector<Batch*> m_batches; // the collection of batches.
		Sprite m_sprite;  // the one sprite instance we modify per draw.
		float m_rotation; // the chosen rotational angle for the next sprite.
		float m_origin_x; // the chosen x origin for the next sprite.
		float m_origin_y; // the chosen y origin for the next sprite.
		float m_scale_x; // the chosen x scale factor for the next sprite.
		float m_scale_y; // the chosen y scale factor for the next sprite.
		LuaColor m_clear_color; // the chosen clear color.

		/* the vertex/index/frame buffer. */
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;
		unsigned int fbo;
		unsigned int fbo_texture;
		Texture* fbo_texture_inst;

		/* a white texture */
		Texture* white_texture;

		/* shader information. */
		unsigned int shader;
		unsigned int shader_transform_loc;
		unsigned int shader_tex_loc;

		/* screen/viewport variables */
		Batch* view_batch;
		glm::mat4 view_proj;
		glm::mat4 screen_proj;
		unsigned int vp_width, vp_height;
		unsigned int sc_width, sc_height;
	};
}

#endif