#ifndef XEN_RENDERER2D_CPP
#define XEN_RENDERER2D_CPP

#include <GLEW/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer2D.h"
#include "../XentuGame.h"


// Specify a macro for storing information about a class and method name,
// this needs to go above any class that will be exposed to lua
#define method(class, name, realname) {#name, &class::realname}

// macro for calculating the byte offset of vertex information.
#define BUFFER_OFFSET(i) ((float*)NULL + (i))


namespace xen
{
	Renderer2D::Renderer2D(lua_State* L) :
		m_rotation(0),
		m_origin_x(0),
		m_origin_y(0),
		m_pos_x(0),
		m_pos_y(0),
		m_scale_x(1),
		m_scale_y(1),
		ibo(-1),
		vbo(-1),
		m_sprite(),
		m_initialized(false),
		m_clear_color(LuaColor::White()),
		m_text_color(Vector4f(1, 1, 1, 1))
	{ }
	
	Renderer2D::~Renderer2D()
	{
		// todo, delete resources created by this class like buffers & textures.
		Advisor::logInfo("Deleted instance of Renderer2D.");
	}


#pragma region Public Methods


	void Renderer2D::initialize(lua_State* L)
	{
		if (!this->m_initialized)
		{
			Advisor::throwOnGLError("initializing renderer");

			XentuGame* game = XentuGame::get_instance(L);
			sc_width = game->config->m_screen_width;
			sc_height = game->config->m_screen_height;
			vp_width = game->config->m_viewport_width;
			vp_height = game->config->m_viewport_height;
			vp_mode = game->config->m_mode;
			view_batch = new Batch();


			// generate a white (10x10) texture.
			unsigned char* wt_data = new unsigned char[400];
			for (int i = 0; i < 400; i++) wt_data[i] = 255;
			unsigned int wt_ref;
			glGenTextures(1, &wt_ref);
			glBindTexture(GL_TEXTURE_2D, wt_ref);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 10, 10, 0, GL_RGBA, GL_UNSIGNED_BYTE, wt_data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			white_texture = new Texture(wt_ref, 10, 10, 4);
			Advisor::throwOnGLError("generating white texture");

			/* calculate the projection matrices */
			glViewport(0, 0, vp_width, vp_height);
			this->view_proj = glm::ortho(0.0f, (float)vp_width, (float)vp_height, 0.0f);
			this->screen_proj = glm::ortho(0.0f, (float)sc_width, (float)sc_height, 0.0f);

			/* get the uniform locations */
			shader = game->shader;
			shader_transform_loc = glGetUniformLocation(shader, "u_MVP");
			Advisor::throwOnGLError("getting camera matrices");
			
			/* set the shader uniforms default */
			glUniformMatrix4fv(shader_transform_loc, 1, false, &view_proj[0][0]);

			/* generate a vertex array object */
			glGenVertexArrays(1, &vao);
			Advisor::throwOnGLError("generating vertex array object");

			// prepare vertex buffer.
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			Advisor::throwOnGLError("generating and binding a vbo");

			// Specify the vertex layout (pos + uv + color).
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, BUFFER_OFFSET(0));
			Advisor::throwOnGLError("setting the 1st shader attribute pointer");
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, BUFFER_OFFSET(3));
			Advisor::throwOnGLError("setting the 2nd shader attribute pointer");
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, BUFFER_OFFSET(5));
			Advisor::throwOnGLError("setting the 3rd shader attribute pointer");

			// Make sure the attributes are marked as enabled.
			glEnable(GL_DEPTH_TEST);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			Advisor::throwOnGLError("enabling the vertex attribute array markers");

			// prepare frame buffer.
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			Advisor::throwOnGLError("generating a frame buffer");

			// prepare frame buffer texture.
			glGenTextures(1, &fbo_texture);
			glBindTexture(GL_TEXTURE_2D, fbo_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp_width, vp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
			Advisor::throwOnGLError("preparing the generated frame buffer");

			// debug the framebuffer creation.
			auto ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (ret != GL_FRAMEBUFFER_COMPLETE) {
				if (ret == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
					Advisor::throwError("Something went wrong with the fbo. GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
				}
				else if (ret == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
					Advisor::throwError("Something went wrong with the fbo. GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
				}
				else {
					Advisor::throwError("Something went wrong with the fbo. #", ret, ", texture #", fbo_texture);
				}
			}
			else {
				Advisor::logInfo("Framebuffer created #", fbo_texture);
			}

			// create a texture for our generated fbo.
			fbo_texture_inst = new Texture(fbo_texture, vp_width, vp_height, 4);
			view_batch = new Batch();
			view_batch->m_texture = fbo_texture_inst;

			// unbind the framebuffer for now.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// create an index buffer.
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			Advisor::throwOnGLError("generating an index buffer");


			
			// set some basic render modes.
			glDisable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			this->m_initialized = true;
		}
	}


	void Renderer2D::draw(const Sprite& sprite)
	{
		if (sprite.m_texture == NULL)
			return;

		find_batch(sprite)->draw(sprite);
	}


	void Renderer2D::order()
	{
		// Sort the batches
		std::sort(m_batches.begin(), m_batches.end(), [&](const Batch* a_batch, const Batch* b_batch)
			{
				if (a_batch->m_layer == b_batch->m_layer)
					return a_batch->m_texture < b_batch->m_texture;

				return false; //a_batch->m_layer < a_batch->m_layer;
			});
	}


	void Renderer2D::clear()
	{
		for (unsigned int i = 0; i < m_batches.size(); i++)
		{
			m_batches[i]->clear();

			if (m_batches[i]->m_inactive > 256)
			{
				std::swap(m_batches[i], m_batches.back());
				delete m_batches.back();
				m_batches.pop_back();
				i--;
			}
		}
	}


#pragma endregion


#pragma region Private Methods


	Batch* Renderer2D::find_batch(const Sprite& sprite)
	{
		for (Batch* batch : m_batches)
		{
			if (batch->m_texture == sprite.m_texture && batch->m_layer == sprite.m_layer)
				return batch;
		}

		Batch* batch = new Batch();
		batch->m_texture = sprite.m_texture;
		batch->m_layer = sprite.m_layer;
		batch->m_inactive = 0;
		batch->quad_count = 0;

		m_batches.push_back(batch);
		return m_batches.back();
	}

	LuaSprite Renderer2D::parse_lua_sprite(lua_State* L)
	{
		LuaSprite s;
		while (lua_next(L, -2)) {
			std::string key = lua_tostring(L, -2);
			if (key == "x")         s.x = lua_tonumber(L, -1);
			if (key == "y")         s.y = lua_tonumber(L, -1);
			if (key == "width")     s.width = lua_tointeger(L, -1);
			if (key == "height")    s.height = lua_tointeger(L, -1);
			if (key == "region")    s.region = lua_tostring(L, -1);
			if (key == "texture")   s.texture = lua_tointeger(L, -1);
			if (key == "spritemap") s.spritemap = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		return s;
	}

	LuaFont Renderer2D::parse_lua_font(lua_State* L)
	{
		LuaFont f = LuaFont::Default();
		while (lua_next(L, -2)) {
			std::string key = lua_tostring(L, -2);
			if (key == "texture") 	f.texture_id = lua_tointeger(L, -1);
			if (key == "spritemap") f.spritemap_id = lua_tointeger(L, -1);
			if (key == "line_height") f.line_height = lua_tointeger(L, -1);
			if (key == "letter_spacing") f.letter_spacing = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
		return f;
	}

	LuaColor Renderer2D::parse_lua_color(lua_State* L)
	{
		LuaColor c = LuaColor::Default();
		while (lua_next(L, -2)) {
			std::string key = lua_tostring(L, -2);
			if (key == "red") c.red = lua_tonumber(L, -1);
			if (key == "green") c.green = lua_tonumber(L, -1);
			if (key == "blue")  c.blue = lua_tonumber(L, -1);
			if (key == "alpha") c.alpha = lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		return c;
	}

	Vector4f Renderer2D::parse_color(lua_State* L)
	{
		float r, g, b, a;
		r = 0;
		g = 0;
		b = 0;
		a = 1;
		while (lua_next(L, -2)) {
			std::string key = lua_tostring(L, -2);
			if (key == "red") r = lua_tonumber(L, -1);
			if (key == "green") g = lua_tonumber(L, -1);
			if (key == "blue")  b = lua_tonumber(L, -1);
			if (key == "alpha") a = lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		return Vector4f(r, g, b, a);
	}


#pragma endregion


#pragma region Lua Accessible Methods


	int Renderer2D::lua_begin(lua_State* L)
	{
		if (!this->m_initialized) {
			this->initialize(L);
		}

		m_origin_x = 0;
		m_origin_y = 0;
		m_pos_x = 0;
		m_pos_y = 0;
		m_scale_x = 1;
		m_scale_y = 1;
		m_rotation = 0;
		//this->clear();

		return 0;
	}


	int Renderer2D::lua_clear(lua_State* L)
	{
		this->clear();
		return 0;
	}


	int Renderer2D::lua_debug_sprite(lua_State* L)
	{
		Advisor::logInfo("--- Sprite Debug ---");
		int count = lua_gettop(L);
		if (count == 1) {
			int type = lua_type(L, 1);
			if (type == LUA_TTABLE) {
				lua_pushnil(L);
				LuaSprite s = parse_lua_sprite(L);
				Advisor::logInfo("sprite: (", s.x, ",", s.y, ",", s.width, ",", s.height, ") [", s.region, "]");
				Advisor::logInfo("success: everything looks normal!");
			}
			else {
				Advisor::throwError("first item on stack is not a table.");
			}
		}
		else {
			Advisor::throwError("no sprite detected.");
		}
		Advisor::logInfo("--- Sprite Debug ---");
		return 0;
	}

	int Renderer2D::lua_draw_sprite(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Invalid Sprite instance passed in arguments");
		}
		// make sure the table is on the stack.
		lua_pushnil(L);
		// iterate the values to populate an LuaSprite struct.
		const LuaSprite s = parse_lua_sprite(L);

		// if the values read from the sprite are duff, lets get out of here.
		if (s.width <= 0 || s.height <= 0) return 0;

		const XentuGame* game = XentuGame::get_instance(L);

		m_sprite.ResetTransform();
		m_sprite.set_position(m_pos_x + s.x, m_pos_y + s.y);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_color = Vector4f(1, 1, 1, 1); // TODO: fix this.
		m_sprite.m_width = s.width;
		m_sprite.m_height = s.height;
		m_sprite.m_texture = game->assets->get_texture(s.texture);

		const SpriteMap* sprite_map = game->assets->get_spritemap(s.spritemap);
		if (sprite_map != NULL) {
			const Rect* r = sprite_map->get_region(s.region);
			if (r != NULL) {
				m_sprite.m_rect = *r;
			}
		}
		else {
			m_sprite.m_rect = Rect(0, 0, 1, 1);
		}

		if (m_sprite.m_texture == NULL)
			return 0;

		find_batch(m_sprite)->draw(m_sprite);

		return 0;
	}

	int Renderer2D::lua_draw_rect(lua_State* L)
	{
		if (lua_gettop(L) != 5) {
			return luaL_error(L, "expecting exactly 4 arguments");
		}
		const int x = lua_tonumber(L, -4);
		const int y = lua_tonumber(L, -3);
		const int width = lua_tonumber(L, -2);
		const int height = lua_tonumber(L, -1);

		// remove the 4 arguments (text, left, top, max_w) from the stack so we can read the font object.
		lua_pop(L, 1);
		lua_pop(L, 1);
		lua_pop(L, 1);
		lua_pop(L, 1);

		// push nil to get the table data to parse.
		lua_pushnil(L);
		const Vector4f color = parse_color(L);

		m_sprite.ResetTransform();
		m_sprite.set_position(x, y);
		m_sprite.set_scale(1, 1);
		m_sprite.set_rotation(0);
		m_sprite.m_color = color; //TODO: stack member duplication.
		m_sprite.m_width = width;
		m_sprite.m_height = height;
		m_sprite.m_texture = white_texture;
		m_sprite.m_rect = Rect(0, 0, 10, 10);
		find_batch(m_sprite)->draw(m_sprite);

		return 1;
	}

	int Renderer2D::lua_draw_text(lua_State* L)
	{
		int arg_count = lua_gettop(L);
		if (arg_count < 4) {
			return luaL_error(L, "expecting exactly 4 or 5 arguments");
		}
		int offset = arg_count == 4 ? 1 : 0;
		// read the other variables first.
		const std::string text = lua_tostring(L, -4 + offset);
		const float left = lua_tonumber(L, -3 + offset);
		const float top = lua_tonumber(L, -2 + offset);
		const float max_width = arg_count == 4 ? 10000 : lua_tonumber(L, -1);

		XentuGame* game = XentuGame::get_instance(L);

		// remove the 4 arguments (text, left, top, max_w) from the stack so we can read the font object.
		lua_pop(L, 1);
		lua_pop(L, 1);
		lua_pop(L, 1);
		if (arg_count == 5) {
			lua_pop(L, 1);
		}

		// push nil to get the table data to parse.
		lua_pushnil(L);
		const LuaFont f = parse_lua_font(L);

		const SpriteMap* sprite_map = game->assets->get_spritemap(f.spritemap_id);

		m_sprite.ResetTransform();
		m_sprite.set_position(left, top);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);
		m_sprite.m_color = this->m_text_color;
		m_sprite.m_width = 10;
		m_sprite.m_height = 22;
		m_sprite.m_texture = game->assets->get_texture(f.texture_id);

		int x_offset = 0, y_offset = 0;
		if (sprite_map != NULL) {
			for (std::string::size_type i = 0; i < text.size(); ++i) {
				std::string cs = std::to_string(text[i]);
				const Rect* rect = sprite_map->get_region(cs);

				if (rect)
				{
					m_sprite.m_width = m_sprite.m_texture->width * rect->width;
					m_sprite.m_height = m_sprite.m_texture->height * rect->height;
					m_sprite.m_rect = *rect;
					find_batch(m_sprite)->draw(m_sprite);
				}

				x_offset += m_sprite.m_width + f.letter_spacing; // 2px is a placeholder

				if (x_offset > max_width && text[i] == 32) {
					x_offset = 0;
					y_offset += f.line_height;
				}

				m_sprite.set_position(left + x_offset, top + y_offset);
			}
		}

		return 0;
	}


	int Renderer2D::lua_draw_tilemap_layer(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const XentuGame* game = XentuGame::get_instance(L);
		TileMapLayer* layer = *static_cast<TileMapLayer**>(lua_touserdata(L, -1));
		
		for (int i=0; i<layer->m_tile_count; i++)
		{
			const auto& tile = layer->m_tiles[i];

			if (tile->texture_id <= 0) continue;

			m_sprite.ResetTransform();
			m_sprite.set_position(m_pos_x + tile->x, m_pos_y + tile->y);
			m_sprite.set_scale(1, 1);
			m_sprite.set_rotation(0);
			//m_sprite.m_color = color; //TODO: stack member duplication.
			m_sprite.m_width = tile->width;
			m_sprite.m_height = tile->height;
			m_sprite.m_texture = game->assets->get_texture(tile->texture_id);

			float u = tile->t_x / (float)m_sprite.m_texture->width;
			float v = tile->t_y / (float)m_sprite.m_texture->height;
			float w = tile->t_width / (float)m_sprite.m_texture->width;
			float h = tile->t_height / (float)m_sprite.m_texture->height;

			m_sprite.m_rect = Rect(u, 1.0 - v - h,w,h);
			find_batch(m_sprite)->draw(m_sprite);
		}


		const int texture_id = layer->get_texture_id();
		if (texture_id > 0) {
			m_sprite.ResetTransform();
			m_sprite.set_position(m_pos_x, m_pos_y);
			m_sprite.set_scale(1, 1);
			m_sprite.set_rotation(0);
			m_sprite.m_color = xen::Vector4f(1, 1, 1, 1); //TODO: stack member duplication.
			m_sprite.m_width = 2048;
			m_sprite.m_height = 2048;
			m_sprite.m_texture = game->assets->get_texture(texture_id);
			m_sprite.m_rect = Rect(0, 0, 1, 1);
			find_batch(m_sprite)->draw(m_sprite);
		}


		const int obj_count = layer->get_object_count();
		if (obj_count > 0) {
			for (int i=0; i<obj_count; i++) {
				const auto obj = layer->get_object(i);
				if (obj->has_tile) {
					const auto tile = obj->get_tile();
					
					if (tile.texture_id <= 0) continue;
					m_sprite.ResetTransform();
					m_sprite.set_position(m_pos_x + obj->x, m_pos_y + obj->y);
					m_sprite.set_scale(1, 1);
					m_sprite.set_rotation(0);
					m_sprite.m_width = tile.width;
					m_sprite.m_height = tile.height;
					m_sprite.m_texture = game->assets->get_texture(tile.texture_id);

					float u = tile.t_x / (float)m_sprite.m_texture->width;
					float v = tile.t_y / (float)m_sprite.m_texture->height;
					float w = tile.t_width / (float)m_sprite.m_texture->width;
					float h = tile.t_height / (float)m_sprite.m_texture->height;

					m_sprite.m_rect = Rect(u, 1.0 - v - h,w,h);
					find_batch(m_sprite)->draw(m_sprite);
				}
			}
		}

		return 0;
	}
	

	int Renderer2D::lua_present(lua_State* L)
	{
		const size_t vertex_size = sizeof(Vertex) * 4; // 4 vertices per quad.
		const size_t element_size = sizeof(unsigned int) * 6; // 6 indices per quad.

		// assign the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// setup the camera.
		glViewport(0, 0, vp_width, vp_height);
		glUniformMatrix4fv(shader_transform_loc, 1, false, &view_proj[0][0]);


		// clear the screen.
		const float r = this->m_clear_color.red;
		const float g = this->m_clear_color.green;
		const float b = this->m_clear_color.blue;
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// draw each of the batches (layers)
		for (const Batch* batch : m_batches)
		{
			const Texture* texture = batch->m_texture;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->gl_texture_id);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, batch->quad_count * vertex_size, batch->m_vertices.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch->quad_count * element_size, batch->m_indices.data(), GL_DYNAMIC_DRAW);

			glDrawElements(GL_TRIANGLES, batch->quad_count * 6, GL_UNSIGNED_INT, nullptr);
		}


		// unbind the frame buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, sc_width, sc_height);
		glUniformMatrix4fv(shader_transform_loc, 1, false, &screen_proj[0][0]);

		// clear the screen once more (this time with black).
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// prepare to draw the render target as a texture.
		m_sprite.ResetTransform();
		m_sprite.ResetTexCoords();
		m_sprite.set_scale(1, 1);

		// choose the viewport mode.
		if (vp_mode == 2) {
			// stretch
			m_sprite.m_width = sc_width;
			m_sprite.m_height = sc_height;
		}
		else if (vp_mode == 1) {
			// centre
			m_sprite.m_width = fbo_texture_inst->width;
			m_sprite.m_height = fbo_texture_inst->height;
			const int hx = (sc_width / 2) - (vp_width / 2);
			const int hy = (sc_height / 2) - (vp_height / 2);
			m_sprite.set_position(hx, hy); 
		}
		else {
			// none
			m_sprite.m_width = fbo_texture_inst->width;
			m_sprite.m_height = fbo_texture_inst->height;
		}
		
		
		m_sprite.m_texture = fbo_texture_inst;
		m_sprite.m_color = Vector4f(1, 1, 1, 1);

		// prepare the view_batch and draw it.
		view_batch->clear();
		view_batch->draw(m_sprite);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo_texture);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, view_batch->quad_count * vertex_size, view_batch->m_vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, view_batch->quad_count * element_size, view_batch->m_indices.data(), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		
		return 0;
	}

	int Renderer2D::lua_set_blend(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const bool enable = lua_toboolean(L, -1);

		if (enable) {
			glEnable(GL_BLEND);
		}
		else {
			glDisable(GL_BLEND);
		}

		return 0;
	}

	int Renderer2D::lua_set_blend_func(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const int sfactor = lua_tonumber(L, -2);
		const int dfactor = lua_tonumber(L, -1);
		glBlendFunc(sfactor, dfactor);
		return 0;
	}
	
	int Renderer2D::lua_set_clear_color(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		// push nil to get the table data to parse.
		lua_pushnil(L);
		const LuaColor f = parse_lua_color(L);
		this->m_clear_color = f;
		return 0;
	}

	int Renderer2D::lua_set_text_color(lua_State* L)
	{
		// push nil to get the table data to parse.
		lua_pushnil(L);
		const Vector4f f = parse_color(L);
		this->m_text_color = f;
		return 0;
	}

	int Renderer2D::lua_set_origin(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		m_origin_x = lua_tonumber(L, -2);
		m_origin_y = lua_tonumber(L, -1);
		return 0;
	}

	int Renderer2D::lua_set_position(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		m_pos_x = lua_tonumber(L, -2);
		m_pos_y = lua_tonumber(L, -1);
		return 0;
	}

	int Renderer2D::lua_set_rotation(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		m_rotation = lua_tonumber(L, -1);
		return 0;
	}

	int Renderer2D::lua_set_scale(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		m_scale_x = lua_tonumber(L, -2);
		m_scale_y = lua_tonumber(L, -1);
		return 0;
	}

	int Renderer2D::lua_set_shader(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const XentuGame* game = XentuGame::get_instance(L);
		int shader_id = lua_isinteger(L, -1) ? lua_tointeger(L, -1) : 0;
		if (shader_id <= 0) {
			shader_id = this->shader;
		}
		glUseProgram(shader_id);

		/* camera stuff */
		shader_transform_loc = glGetUniformLocation(shader, "u_MVP"); // todo, change this

		/* texture prep */
		shader_tex_loc = glGetUniformLocation(shader, "u_Texture");

		Advisor::logInfo("Using shader #", shader_id);
		return 0;
	}


#pragma endregion


	const char Renderer2D::className[] = "Renderer2D";

	const Luna<Renderer2D>::PropertyType Renderer2D::properties[] = {
		{0,0}
	};

	const Luna<Renderer2D>::FunctionType Renderer2D::methods[] = {
		method(Renderer2D, begin, lua_begin),
		method(Renderer2D, clear, lua_clear),
		method(Renderer2D, debug_sprite, lua_debug_sprite),
		method(Renderer2D, draw_sprite, lua_draw_sprite),
		method(Renderer2D, draw_rect, lua_draw_rect),
		method(Renderer2D, draw_text, lua_draw_text),
		method(Renderer2D, draw_tilemap_layer, lua_draw_tilemap_layer),
		method(Renderer2D, present, lua_present),
		method(Renderer2D, set_blend, lua_set_blend),
		method(Renderer2D, set_blend_func, lua_set_blend_func),
		method(Renderer2D, set_clear_color, lua_set_clear_color),
		method(Renderer2D, set_text_color, lua_set_text_color),
		method(Renderer2D, set_origin, lua_set_origin),
		method(Renderer2D, set_position, lua_set_position),
		method(Renderer2D, set_rotation, lua_set_rotation),
		method(Renderer2D, set_scale, lua_set_scale),
		method(Renderer2D, set_shader, lua_set_shader),
		{0,0}
	};
}

#endif