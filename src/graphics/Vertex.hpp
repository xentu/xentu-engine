#ifndef XEN_VERTEX_HPP
#define XEN_VERTEX_HPP

#include "Color.hpp"
#include "Vector2.hpp"

namespace xen
{
	struct Vertex
	{
		Vertex();
		Vertex(const Vector2f& position);
		Vertex(const Vector2f& position, const Vector2f& texCoords);
		Vertex(float x, float y, float u, float v);

		Vector2f position;
		Vector2f texCoords;
	};
}

#endif