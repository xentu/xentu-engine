#ifndef XEN_COLOR_CPP
#define XEN_COLOR_CPP

#include "Color.hpp"

namespace xen
{
	const Color Color::black(0, 0, 0);
	const Color Color::white(255, 255, 255);
	const Color Color::red(255, 0, 0);
	const Color Color::green(0, 255, 0);
	const Color Color::blue(0, 0, 255);
	const Color Color::yellow(255, 255, 0);
	const Color Color::magenta(255, 0, 255);
	const Color Color::cyan(0, 255, 255);
	const Color Color::transparent(0, 0, 0, 0);



	Color::Color() :
		r(0),
		g(0),
		b(0),
		a(255)
	{ }



	Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
		r(red),
		g(green),
		b(blue),
		a(alpha)
	{ }



	Color::Color(signed int color) :
		r((color & 0xff000000) >> 24),
		g((color & 0x00ff0000) >> 16),
		b((color & 0x0000ff00) >> 8),
		a((color & 0x000000ff) >> 0)
	{ }



	signed int Color::to_integer() const
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}



	inline bool operator ==(const Color& left, const Color& right)
	{
		return left.r == right.r
			&& left.g == right.g
			&& left.b == right.b
			&& left.a == right.a;
	}



	inline bool operator !=(const Color& left, const Color& right)
	{
		return left.r != right.r
			&& left.g != right.g
			&& left.b != right.b
			&& left.a != right.a;
	}
}

#endif