#ifndef XEN_COLOR_CPP
#define XEN_COLOR_CPP

#include "Vertex.h"

namespace xen
{
	Vertex::Vertex() :
		position(Vector2f(0, 0)),
		texCoords(Vector2f(0, 0)),
		color(Vector4f(1, 1, 1, 1))
	{ }



	Vertex::Vertex(const Vector2f& position) :
		position(position),
		texCoords(Vector2f(0, 0)),
		color(Vector4f(1, 1, 1, 1))
	{ }



	Vertex::Vertex(const Vector2f& position, const Vector2f& texCoords) :
		position(position),
		texCoords(texCoords),
		color(Vector4f(1, 1, 1, 1))
	{ }



	Vertex::Vertex(float x, float y, float u, float v) :
		position(Vector2f(x, y)),
		texCoords(Vector2f(u, v)),
		color(Vector4f(1, 1, 1, 1))
	{ }
}

#endif