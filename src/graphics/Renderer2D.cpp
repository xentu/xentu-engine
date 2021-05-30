#ifndef XEN_SPRITE_BATCH_CPP
#define XEN_SPRITE_BATCH_CPP

#include <GLEW/GL/glew.h>

#include "Renderer2D.hpp"
#include "../XentuGame.hpp"

// Specify a macro for storing information about a class and method name, 
// this needs to go above any class that will be exposed to lua
#define method(class, name) {#name, &class::name}

namespace xen
{
	Renderer2D::Renderer2D(lua_State* L) :
		m_rotation(0),
		m_origin_x(0),
		m_origin_y(0),
		m_scale_x(1),
		m_scale_y(1),
		ibo(-1),
		vbo(-1),
		m_sprite(),
		m_initialized(false)
	{ }



	Renderer2D::~Renderer2D()
	{
		std::cout << "Deleted instance of Renderer2D." << std::endl;
	}


// macro for calculating the byte offset of vertex information.
#define BUFFER_OFFSET(i) ((char*)NULL + (i))



	void Renderer2D::initialize()
	{
		if (!this->m_initialized)
		{
			// prepare vertex buffer.
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			// prepare index buffer.
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			/* Specify the vertex layout (pos + uv). */
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(8));

			this->m_initialized = true;
		}
	}



	int Renderer2D::begin(lua_State* L)
	{
		if (!this->m_initialized) {
			this->initialize();
		}

		m_origin_x = 0;
		m_origin_y = 0;
		m_scale_x = 1;
		m_scale_y = 1;
		m_rotation = 0;
		this->clear();
		return 1;
	}



	int Renderer2D::present(lua_State* L)
	{
		const int vertex_size = 4 * sizeof(Vertex);
		const int element_size = 6 * sizeof(unsigned int);

		//glClearColor(1, 1, 1, 1);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		for (const Batch* batch : m_batches)
		{
			const Texture* texture = batch->m_texture;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->gl_texture_id);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, batch->m_count * vertex_size, batch->m_vertices.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch->m_count * element_size, batch->m_indices.data(), GL_DYNAMIC_DRAW);			
			
			// draw stuff.
			glDrawElements(GL_TRIANGLES, batch->m_count * 6, GL_UNSIGNED_INT, nullptr);
			//glDrawArrays(GL_TRIANGLES, 0, batch->m_count * 6);
		}

		return 1;
	}



	int Renderer2D::draw_sprite(lua_State* L)
	{
		int	texture_id = lua_tointeger(L, -6);
		int	spritemap_id = lua_tointeger(L, -6);
		std::string sprite_name = lua_tostring(L, -5);
		float left = lua_tonumber(L, -4);
		float top = lua_tonumber(L, -3);
		float width = lua_tonumber(L, -2);
		float height = lua_tonumber(L, -1);

		XentuGame* game = XentuGame::get_instance(L);

		m_sprite.ResetTransform();
		m_sprite.set_position(left, top);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_width = width;
		m_sprite.m_height = height;
		m_sprite.m_texture = game->assets->get_texture(texture_id);

		SpriteMap* sprite_map = game->assets->get_spritemap(spritemap_id);
		if (sprite_map != NULL) {
			Rect* r = sprite_map->get_region(sprite_name);
			m_sprite.m_rect = *r;
		}

		if (m_sprite.m_texture == NULL)
			return 0;

		find_batch(m_sprite)->draw(m_sprite);

		return 1;
	}


	int Renderer2D::draw_text(lua_State* L)
	{
		int texture_id = lua_tointeger(L, -5);
		int spritemap_id = lua_tointeger(L, -5);
		std::string text = lua_tostring(L, -4);
		float left = lua_tonumber(L, -3);
		float top = lua_tonumber(L, -2);
		//float max_width = lua_tonumber(L, -1);

		XentuGame* game = XentuGame::get_instance(L);
		SpriteMap* sprite_map = game->assets->get_spritemap(spritemap_id);

		m_sprite.ResetTransform();
		m_sprite.set_position(left, top);
		m_sprite.set_origin(m_origin_x, m_origin_y);
		m_sprite.set_rotation(m_rotation);
		m_sprite.set_scale(m_scale_x, m_scale_y);

		m_sprite.m_width = 10;
		m_sprite.m_height = 22;
		m_sprite.m_texture = game->assets->get_texture(texture_id);

		
		int x_offset = 0;
		if (sprite_map != NULL) {

			for(std::string::size_type i = 0; i < text.size(); ++i) {
				std::string cs = std::to_string(text[i]);
				Rect* r = sprite_map->get_region(cs);

				if (r)
				{
					m_sprite.m_rect = *r;
					find_batch(m_sprite)->draw(m_sprite);
				}
				
				x_offset += 15;
				m_sprite.set_position(left + x_offset, top);
			}
		}
		
		return 0;
	}



	int Renderer2D::set_origin(lua_State* L)
	{
		m_origin_x = lua_tonumber(L, -2);
		m_origin_y = lua_tonumber(L, -1);
		return 1;
	}



	int Renderer2D::set_rotation(lua_State* L)
	{
		m_rotation = lua_tonumber(L, -1);
		return 1;
	}



	int Renderer2D::set_scale(lua_State* L)
	{
		m_scale_x = lua_tonumber(L, -2);
		m_scale_y = lua_tonumber(L, -1);
		return 1;
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
		batch->m_count = 0;

		m_batches.push_back(batch);
		return m_batches.back();
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



	const char Renderer2D::className[] = "Renderer2D";



	const Luna<Renderer2D>::PropertyType Renderer2D::properties[] = {
		{0,0}
	};



	const Luna<Renderer2D>::FunctionType Renderer2D::methods[] = {
		method(Renderer2D, begin),
		method(Renderer2D, draw_sprite),
		method(Renderer2D, draw_text),
		method(Renderer2D, present),
		method(Renderer2D, set_rotation),
		method(Renderer2D, set_origin),
		method(Renderer2D, set_scale),
		{0,0}
	};
}


#endif