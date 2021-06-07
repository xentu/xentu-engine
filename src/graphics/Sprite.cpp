#ifndef XEN_SPRITE_CPP
#define XEN_SPRITE_CPP

#include "Sprite.h"

namespace xen
{
	Sprite::Sprite()
	{
		m_texture = nullptr;
		m_color = Color::white;
		m_layer = 0;
		m_rect = Rect(0, 0, 1, 1);
	}



	Sprite::Sprite(const Texture& texture, float width, float height, int layer, Color color)
	{
		m_texture = &texture;
		m_width = 0;
		m_height = 0;
		m_layer = layer;
		m_color = color;
		m_rect = Rect(0, 0, 1, 1);
	}


	void Sprite::ResetTexCoords()
	{
		m_rect.left = 0;
		m_rect.top = 0;
		m_rect.width = 1;
		m_rect.height = 1;
	}
}

#endif