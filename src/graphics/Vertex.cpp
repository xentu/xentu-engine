#include "Vertex.h"

namespace xen
{
	Vertex::Vertex() :
		position(Vector3f(0, 0, 0)),
		texCoords(Vector2f(0, 0)),
		color(Vector4f(1, 1, 1, 1))
	{ }



	Vertex::Vertex(const Vector3f& position) :
		position(position),
		texCoords(Vector2f(0, 0)),
		color(Vector4f(1, 1, 1, 1))
	{ }



	Vertex::Vertex(const Vector3f& position, const Vector2f& texCoords) :
		position(position),
		texCoords(texCoords),
		color(Vector4f(1, 1, 1, 1))
	{ }



	Vertex::Vertex(float x, float y, float z, float u, float v) :
		position(Vector3f(x, y, z)),
		texCoords(Vector2f(u, v)),
		color(Vector4f(1, 1, 1, 1))
	{ }
}