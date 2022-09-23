#include "Sprite.h"

namespace xen
{
	Sprite::Sprite()
	{
		m_texture = nullptr;
		m_color = Vector4f(1, 1, 1, 1);
		m_layer = 0;
		m_rect = Rect(0, 0, 1, 1);
	}



	Sprite::Sprite(const Texture& texture, float width, float height, Vector4f color, int layer)
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