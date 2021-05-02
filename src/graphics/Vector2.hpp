#ifndef XEN_VECTOR2_HPP
#define XEN_VECTOR2_HPP

namespace xen
{
    template <typename T>
    struct Vector2
    {
    public:
        Vector2();
        Vector2(T X, T Y);

        template <typename U>
        explicit Vector2(const Vector2<U>& vector);

        T x; //!< X coordinate of the vector
        T y; //!< Y coordinate of the vector
    };

    template <typename T>
    Vector2<T> operator -(const Vector2<T>& right);

    template <typename T>
    Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);

    template <typename T>
    Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);

    template <typename T>
    Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right);

    template <typename T>
    Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);

    template <typename T>
    Vector2<T> operator *(const Vector2<T>& left, T right);

    template <typename T>
    Vector2<T> operator *(T left, const Vector2<T>& right);

    template <typename T>
    Vector2<T>& operator *=(Vector2<T>& left, T right);

    template <typename T>
    Vector2<T> operator /(const Vector2<T>& left, T right);

    template <typename T>
    Vector2<T>& operator /=(Vector2<T>& left, T right);

    template <typename T>
    bool operator ==(const Vector2<T>& left, const Vector2<T>& right);

    template <typename T>
    bool operator !=(const Vector2<T>& left, const Vector2<T>& right);



    // -------------------------------------------------------------- inlines


    template <typename T>
    inline Vector2<T>::Vector2() :
        x(0),
        y(0)
    { }


    template <typename T>
    inline Vector2<T>::Vector2(T X, T Y) :
        x(X),
        y(Y)
    { }


    template <typename T>
    template <typename U>
    inline Vector2<T>::Vector2(const Vector2<U>& vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y))
    { }


    template <typename T>
    inline Vector2<T> operator -(const Vector2<T>& right)
    {
        return Vector2<T>(-right.x, -right.y);
    }


    template <typename T>
    inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
    {
        left.x += right.x;
        left.y += right.y;

        return left;
    }


    template <typename T>
    inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
    {
        left.x -= right.x;
        left.y -= right.y;

        return left;
    }


    template <typename T>
    inline Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
    {
        return Vector2<T>(left.x + right.x, left.y + right.y);
    }


    template <typename T>
    inline Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
    {
        return Vector2<T>(left.x - right.x, left.y - right.y);
    }


    template <typename T>
    inline Vector2<T> operator *(const Vector2<T>& left, T right)
    {
        return Vector2<T>(left.x * right, left.y * right);
    }


    template <typename T>
    inline Vector2<T> operator *(T left, const Vector2<T>& right)
    {
        return Vector2<T>(right.x * left, right.y * left);
    }


    template <typename T>
    inline Vector2<T>& operator *=(Vector2<T>& left, T right)
    {
        left.x *= right;
        left.y *= right;

        return left;
    }


    template <typename T>
    inline Vector2<T> operator /(const Vector2<T>& left, T right)
    {
        return Vector2<T>(left.x / right, left.y / right);
    }


    template <typename T>
    inline Vector2<T>& operator /=(Vector2<T>& left, T right)
    {
        left.x /= right;
        left.y /= right;

        return left;
    }


    template <typename T>
    inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
    {
        return (left.x == right.x) && (left.y == right.y);
    }


    template <typename T>
    inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
    {
        return (left.x != right.x) || (left.y != right.y);
    }


    // -------------------------------------------------------------- inlines

    // Define the most common types
    typedef Vector2<int>          Vector2i;
    typedef Vector2<unsigned int> Vector2u;
    typedef Vector2<float>        Vector2f;
}

#endif