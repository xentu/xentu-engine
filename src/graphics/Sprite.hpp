#ifndef XEN_SPRITE_HPP
#define XEN_SPRITE_HPP

#include "Color.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Transformable.hpp"

namespace xen
{
	struct Sprite : public Transformable
	{
		Sprite();
		Sprite(const Texture& texture, float width, float height, int layer = 0, Color color = Color::white);
		void ResetTexCoords();

		const Texture* m_texture;
		Color m_color;
		float m_width;
		float m_height;
		int m_layer;
		Rect m_rect;
	};
}

#endif