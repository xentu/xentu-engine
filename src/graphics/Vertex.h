#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace xen
{
	struct Vertex
	{
		Vertex();
		Vertex(const Vector3f& position);
		Vertex(const Vector3f& position, const Vector2f& texCoords);
		Vertex(float x, float y, float z, float u, float v);

		Vector3f position;
		Vector2f texCoords;
		Vector4f color;
	};
}