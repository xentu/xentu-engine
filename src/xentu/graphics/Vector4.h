#ifndef XEN_VECTOR4_HPP
#define XEN_VECTOR4_HPP

namespace xen
{
	template <typename T>
	struct Vector4
	{
	public:
		Vector4();
		Vector4(T X, T Y, T Z, T W);

		template <typename U>
		explicit Vector4(const Vector4<U>& vector);

		T x; //!< X coordinate of the vector
		T y; //!< Y coordinate of the vector
		T z; //!< Z coordinate of the vector
		T w; //!< W coordinate of the vector
	};

	template <typename T>
	Vector4<T> operator -(const Vector4<T>& right);

	template <typename T>
	Vector4<T>& operator +=(Vector4<T>& left, const Vector4<T>& right);

	template <typename T>
	Vector4<T>& operator -=(Vector4<T>& left, const Vector4<T>& right);

	template <typename T>
	Vector4<T> operator +(const Vector4<T>& left, const Vector4<T>& right);

	template <typename T>
	Vector4<T> operator -(const Vector4<T>& left, const Vector4<T>& right);

	template <typename T>
	Vector4<T> operator *(const Vector4<T>& left, T right);

	template <typename T>
	Vector4<T> operator *(T left, const Vector4<T>& right);

	template <typename T>
	Vector4<T>& operator *=(Vector4<T>& left, T right);

	template <typename T>
	Vector4<T> operator /(const Vector4<T>& left, T right);

	template <typename T>
	Vector4<T>& operator /=(Vector4<T>& left, T right);

	template <typename T>
	bool operator ==(const Vector4<T>& left, const Vector4<T>& right);

	template <typename T>
	bool operator !=(const Vector4<T>& left, const Vector4<T>& right);



	// -------------------------------------------------------------- inlines


	template <typename T>
	inline Vector4<T>::Vector4() :
		x(0),
		y(0),
		z(0),
		w(0)
	{ }


	template <typename T>
	inline Vector4<T>::Vector4(T X, T Y, T Z, T W) :
		x(X),
		y(Y),
		z(Z),
		w(W)
	{ }


	template <typename T>
	template <typename U>
	inline Vector4<T>::Vector4(const Vector4<U>& vector) :
		x(static_cast<T>(vector.x)),
		y(static_cast<T>(vector.y)),
		z(static_cast<T>(vector.z)),
		w(static_cast<T>(vector.w))
	{ }


	template <typename T>
	inline Vector4<T> operator -(const Vector4<T>& right)
	{
		return Vector4<T>(-right.x, -right.y, -right.z, -right.w);
	}


	template <typename T>
	inline Vector4<T>& operator +=(Vector4<T>& left, const Vector4<T>& right)
	{
		left.x += right.x;
		left.y += right.y;
		left.z += right.z;
		left.w += right.w;
		return left;
	}


	template <typename T>
	inline Vector4<T>& operator -=(Vector4<T>& left, const Vector4<T>& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		left.z -= right.z;
		left.w -= right.w;
		return left;
	}


	template <typename T>
	inline Vector4<T> operator +(const Vector4<T>& left, const Vector4<T>& right)
	{
		return Vector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}


	template <typename T>
	inline Vector4<T> operator -(const Vector4<T>& left, const Vector4<T>& right)
	{
		return Vector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}


	template <typename T>
	inline Vector4<T> operator *(const Vector4<T>& left, T right)
	{
		return Vector4<T>(left.x * right, left.y * right, left.z * right, left.w * right);
	}


	template <typename T>
	inline Vector4<T> operator *(T left, const Vector4<T>& right)
	{
		return Vector4<T>(right.x * left, right.y * left, right.z * left, right.w * left);
	}


	template <typename T>
	inline Vector4<T>& operator *=(Vector4<T>& left, T right)
	{
		left.x *= right;
		left.y *= right;
		left.z *= right;
		left.w *= right;
		return left;
	}


	template <typename T>
	inline Vector4<T> operator /(const Vector4<T>& left, T right)
	{
		return Vector4<T>(left.x / right, left.y / right, left.z / right, left.w / right);
	}


	template <typename T>
	inline Vector4<T>& operator /=(Vector4<T>& left, T right)
	{
		left.x /= right;
		left.y /= right;
		left.z /= right;
		left.w /= right;
		return left;
	}


	template <typename T>
	inline bool operator ==(const Vector4<T>& left, const Vector4<T>& right)
	{
		return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
	}


	template <typename T>
	inline bool operator !=(const Vector4<T>& left, const Vector4<T>& right)
	{
		return (left.x != right.x) || (left.y != right.y) || (left.z != right.y) || (left.w != right.w);
	}


	// -------------------------------------------------------------- inlines
	
	// Define the most common types
	typedef Vector4<int>          Vector4i;
	typedef Vector4<unsigned int> Vector4u;
	typedef Vector4<float>        Vector4f;
}

#endif