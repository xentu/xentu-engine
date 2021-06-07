#ifndef XEN_COLOR_HPP
#define XEN_COLOR_HPP

namespace xen
{
	class Color
	{
	public:
		static const Color black;       //!< Black predefined color
		static const Color white;       //!< White predefined color
		static const Color red;         //!< Red predefined color
		static const Color green;       //!< Green predefined color
		static const Color blue;        //!< Blue predefined color
		static const Color yellow;      //!< Yellow predefined color
		static const Color magenta;     //!< Magenta predefined color
		static const Color cyan;        //!< Cyan predefined color
		static const Color transparent; //!< Transparent (black) predefined color


		Color();
		Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
		Color(signed int color);

		signed int to_integer() const;

		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};


	bool operator ==(const Color& left, const Color& right);
}

#endif