#ifndef XEN_RECT_HPP
#define XEN_RECT_HPP

#include <algorithm>
#include <cmath>

#include "Vector2.hpp"

namespace xen
{
    class Rect
    {
    public:
        Rect();
        Rect(float rectLeft, float rectTop, float rectWidth, float rectHeight);
        Rect(const Vector2f& position, const Vector2f& size);

        //explicit Rect(const Rect& rectangle);
        Rect(const Rect& rectangle);

        bool contains(float x, float y) const;
        bool contains(const Vector2f& point) const;
        bool intersects(const Rect& rectangle) const;
        bool intersects(const Rect& rectangle, Rect& intersection) const;

        Vector2f get_position() const;
        Vector2f get_size() const;

        float left;   //!< Left coordinate of the rectangle
        float top;    //!< Top coordinate of the rectangle
        float width;  //!< Width of the rectangle
        float height; //!< Height of the rectangle
    };
}

#endif