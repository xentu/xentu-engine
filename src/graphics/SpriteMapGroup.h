#pragma once

#include <map>
#include <string>

#include "Rect.h"

namespace xen
{
	struct SpriteMapFrame
	{
		Rect* coords;
		int delay;
		bool flip_x;
		bool flip_y;
	};


	class SpriteMapGroup
	{
	public:
		/// <summary>
		/// Create a new asset manager instance.
		/// </summary>
		SpriteMapGroup();
		~SpriteMapGroup();


		const SpriteMapFrame* get_frame(int frame) const;
		int add_frame(SpriteMapFrame* frame);
		int get_count() const;


	private:
		// the asset_id for the loaded texture.
		int m_frames_count;
		SpriteMapFrame *m_frames[255];
	};
}