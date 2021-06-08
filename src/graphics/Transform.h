#ifndef XEN_TRANSFORM_HPP
#define XEN_TRANSFORM_HPP

#include "Rect.h"
#include "Vector2.h"

namespace xen
{
	class Transform
	{
	public:
		Transform();
		Transform(float a00, float a01, float a02,
			float a10, float a11, float a12,
			float a20, float a21, float a22);

		const float* get_matrix() const;

		Transform get_inverse() const;
		Vector2f transform_point(float x, float y) const;
		Vector2f transform_point(const Vector2f& point) const;
		Rect transform_rect(const Rect& rectangle) const;
		Transform& combine(const Transform& transform);
		Transform& translate(float x, float y);
		Transform& translate(const Vector2f& offset);
		Transform& rotate(float angle);
		Transform& rotate(float angle, float centerX, float centerY);
		Transform& rotate(float angle, const Vector2f& center);
		Transform& scale(float scaleX, float scaleY);
		Transform& scale(float scaleX, float scaleY, float centerX, float centerY);
		Transform& scale(const Vector2f& factors);
		Transform& scale(const Vector2f& factors, const Vector2f& center);
		static const Transform Identity; //!< The identity transform (does nothing)

	private:
		float m_matrix[16]; //!< 4x4 matrix defining the transformation
	};


	Transform operator *(const Transform& left, const Transform& right);
	Transform& operator *=(Transform& left, const Transform& right);
	Vector2f operator *(const Transform& left, const Vector2f& right);
	bool operator ==(const Transform& left, const Transform& right);
	bool operator !=(const Transform& left, const Transform& right);
}

#endif