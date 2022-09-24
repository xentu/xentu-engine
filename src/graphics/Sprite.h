#pragma once

#include <string>
#include "Vector4.h"
#include "Rect.h"
#include "Texture.h"
#include "Transformable.h"

namespace xen
{
	struct Sprite : public Transformable
	{
		Sprite();
		Sprite(GLuint texture, float width, float height, Vector4f color, int layer = 0);
		void ResetTexCoords();

		GLuint m_texture;
		Vector4f m_color;
		float m_width;
		float m_height;
		int m_layer;
		Rect m_rect;
	};
}