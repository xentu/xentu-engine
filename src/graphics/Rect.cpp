#ifndef XEN_RECT_CPP
#define XEN_RECT_CPP

#include "Rect.h"

namespace xen
{
    Rect::Rect() :
        left(0),
        top(0),
        width(0),
        height(0)
    { }



    Rect::Rect(float rectLeft, float rectTop, float rectWidth, float rectHeight) :
        left(rectLeft),
        top(rectTop),
        width(rectWidth),
        height(rectHeight)
    { }



    Rect::Rect(const Vector2f& position, const Vector2f& size) :
        left(position.x),
        top(position.y),
        width(size.x),
        height(size.y)
    { }



    Rect::Rect(const Rect& rectangle) :
        left(rectangle.left),
        top(rectangle.top),
        width(rectangle.width),
        height(rectangle.height)
    { }



    bool Rect::contains(float x, float y) const
    {
        // Rectangles with negative dimensions are allowed, so we must handle them correctly

        // Compute the real min and max of the rectangle on both axes
        float minX = std::min(left, left + width);
        float maxX = std::max(left, left + width);
        float minY = std::min(top, top + height);
        float maxY = std::max(top, top + height);

        return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
    }



    bool Rect::contains(const Vector2f& point) const
    {
        return contains(point.x, point.y);
    }



    bool Rect::intersects(const Rect& rectangle) const
    {
        Rect intersection;
        return intersects(rectangle, intersection);
    }



    bool Rect::intersects(const Rect& rectangle, Rect& intersection) const
    {
        // Rectangles with negative dimensions are allowed, so we must handle them correctly

        // Compute the min and max of the first rectangle on both axes
        float r1MinX = std::min(left, left + width);
        float r1MaxX = std::max(left, left + width);
        float r1MinY = std::min(top, top + height);
        float r1MaxY = std::max(top, top + height);

        // Compute the min and max of the second rectangle on both axes
        float r2MinX = std::min(rectangle.left, rectangle.left + rectangle.width);
        float r2MaxX = std::max(rectangle.left, rectangle.left + rectangle.width);
        float r2MinY = std::min(rectangle.top, rectangle.top + rectangle.height);
        float r2MaxY = std::max(rectangle.top, rectangle.top + rectangle.height);

        // Compute the intersection boundaries
        float interLeft = std::max(r1MinX, r2MinX);
        float interTop = std::max(r1MinY, r2MinY);
        float interRight = std::min(r1MaxX, r2MaxX);
        float interBottom = std::min(r1MaxY, r2MaxY);

        // If the intersection is valid (positive non zero area), then there is an intersection
        if ((interLeft < interRight) && (interTop < interBottom))
        {
            intersection = Rect(interLeft, interTop, interRight - interLeft, interBottom - interTop);
            return true;
        }
        else
        {
            intersection = Rect(0, 0, 0, 0);
            return false;
        }
    }



    Vector2<float> Rect::get_position() const
    {
        return Vector2<float>(left, top);
    }



    Vector2<float> Rect::get_size() const
    {
        return Vector2<float>(width, height);
    }



    inline bool operator ==(const Rect& left, const Rect& right)
    {
        return (left.left == right.left) && (left.width == right.width) &&
            (left.top == right.top) && (left.height == right.height);
    }



    inline bool operator !=(const Rect& left, const Rect& right)
    {
        return !(left == right);
    }
}

#endif