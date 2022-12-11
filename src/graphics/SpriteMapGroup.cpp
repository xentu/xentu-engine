#include <iostream>
#include <sstream>
#include <fstream>

#include <json/json.hpp>
#include "SpriteMapGroup.h"

namespace xen
{
	SpriteMapGroup::SpriteMapGroup()
	{
		m_frames_count = 0;
	}


	SpriteMapGroup::~SpriteMapGroup()
	{

	}


	const SpriteMapFrame* SpriteMapGroup::get_frame(int frame) const
	{
		return m_frames[frame];
	}


	int SpriteMapGroup::add_frame(SpriteMapFrame* frame)
	{
		m_frames[m_frames_count] = frame;
		m_frames_count++;
		return -1;
	}


	int SpriteMapGroup::get_count() const
	{
		return m_frames_count;
	}
}