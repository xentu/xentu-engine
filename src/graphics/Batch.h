#pragma once

#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "Sprite.h"
#include "Vertex.h"

namespace xen
{
	struct Batch
	{
		void clear()
		{
			if (quad_count > 0)
			{
				quad_count = 0;
				m_inactive = 0;
			}
			else
				m_inactive++;
		}


		void draw(const Sprite& sprite)
		{
			size_t vertex_count = quad_count * 4;
			size_t index_count = quad_count * 6;

			if (vertex_count + 4 > m_vertices.size()) {
				m_vertices.resize(m_vertices.size() + 4);
				m_indices.resize(m_indices.size() + 6);
			}

			Vector3f position(0, 0, 0);
			Vector3f size(sprite.m_width, sprite.m_height, 0);

			Transform transform = sprite.get_transform();

			// get the texture transform.
			//auto tex_tl = Vector2f(sprite.m_rect.left, sprite.m_rect.top);
			//auto tex_tr = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top);
			//auto tex_bl = Vector2f(sprite.m_rect.left, sprite.m_rect.top + sprite.m_rect.height);
			//auto tex_br = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top + sprite.m_rect.height);

			auto tex_tl = Vector2f(sprite.m_rect.left, sprite.m_rect.top + sprite.m_rect.height);
			auto tex_tr = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top + sprite.m_rect.height);
			auto tex_bl = Vector2f(sprite.m_rect.left, sprite.m_rect.top);
			auto tex_br = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top);

			// assign the vertices.
			m_vertices[vertex_count].position = transform.transform_point(0.0f, 0.0f, 0);
			m_vertices[vertex_count].texCoords = tex_bl;
			m_vertices[vertex_count].color = sprite.m_color;

			m_vertices[vertex_count + 1].position = transform.transform_point(size.x, 0.0f, 0);
			m_vertices[vertex_count + 1].texCoords = tex_br;
			m_vertices[vertex_count + 1].color = sprite.m_color;

			m_vertices[vertex_count + 2].position = transform.transform_point(size);
			m_vertices[vertex_count + 2].texCoords = tex_tr;
			m_vertices[vertex_count + 2].color = sprite.m_color;

			m_vertices[vertex_count + 3].position = transform.transform_point(0.0f, size.y, 0);
			m_vertices[vertex_count + 3].texCoords = tex_tl;
			m_vertices[vertex_count + 3].color = sprite.m_color;

			// assign the indices.
			m_indices[index_count] = vertex_count;
			m_indices[index_count + 1] = vertex_count + 1;
			m_indices[index_count + 2] = vertex_count + 2;
			m_indices[index_count + 3] = vertex_count + 2;
			m_indices[index_count + 4] = vertex_count + 3;
			m_indices[index_count + 5] = vertex_count;

			quad_count++;
		}

		void draw_inverted(const Sprite& sprite)
		{
			size_t vertex_count = quad_count * 4;
			size_t index_count = quad_count * 6;

			if (vertex_count + 4 > m_vertices.size()) {
				m_vertices.resize(m_vertices.size() + 4);
				m_indices.resize(m_indices.size() + 6);
			}

			Vector3f position(0, 0, 0);
			Vector3f size(sprite.m_width, sprite.m_height, 0);

			Transform transform = sprite.get_transform();

			// get the texture transform.
			auto tex_tl = Vector2f(sprite.m_rect.left, sprite.m_rect.top);
			auto tex_tr = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top);
			auto tex_bl = Vector2f(sprite.m_rect.left, sprite.m_rect.top + sprite.m_rect.height);
			auto tex_br = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top + sprite.m_rect.height);

			//auto tex_tl = Vector2f(sprite.m_rect.left, sprite.m_rect.top + sprite.m_rect.height);
			//auto tex_tr = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top + sprite.m_rect.height);
			//auto tex_bl = Vector2f(sprite.m_rect.left, sprite.m_rect.top);
			//auto tex_br = Vector2f(sprite.m_rect.left + sprite.m_rect.width, sprite.m_rect.top);

			// assign the vertices.
			m_vertices[vertex_count].position = transform.transform_point(0.0f, 0.0f, 0);
			m_vertices[vertex_count].texCoords = tex_bl;
			m_vertices[vertex_count].color = sprite.m_color;

			m_vertices[vertex_count + 1].position = transform.transform_point(size.x, 0.0f, 0);
			m_vertices[vertex_count + 1].texCoords = tex_br;
			m_vertices[vertex_count + 1].color = sprite.m_color;

			m_vertices[vertex_count + 2].position = transform.transform_point(size);
			m_vertices[vertex_count + 2].texCoords = tex_tr;
			m_vertices[vertex_count + 2].color = sprite.m_color;

			m_vertices[vertex_count + 3].position = transform.transform_point(0.0f, size.y, 0);
			m_vertices[vertex_count + 3].texCoords = tex_tl;
			m_vertices[vertex_count + 3].color = sprite.m_color;

			// assign the indices.
			m_indices[index_count] = vertex_count;
			m_indices[index_count + 1] = vertex_count + 1;
			m_indices[index_count + 2] = vertex_count + 2;
			m_indices[index_count + 3] = vertex_count + 2;
			m_indices[index_count + 4] = vertex_count + 3;
			m_indices[index_count + 5] = vertex_count;

			quad_count++;
		}


		GLuint m_texture;
		int m_layer;
		unsigned int m_inactive;
		unsigned int quad_count;
		std::vector<unsigned int> m_indices;
		std::vector<Vertex> m_vertices;
	};
}