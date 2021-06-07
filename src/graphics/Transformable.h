#ifndef XEN_TRANSFORMABLE_HPP
#define XEN_TRANSFORMABLE_HPP

#include "Transform.h"
#include "Vector2.h"

namespace xen
{
    class Transformable
    {
    public:
        Transformable();
        virtual ~Transformable();
        void set_position(float x, float y);
        void set_position(const Vector2f& position);
        void set_rotation(float angle);
        void set_scale(float factorX, float factorY);
        void set_scale(const Vector2f& factors);
        void set_origin(float x, float y);
        void set_origin(const Vector2f& origin);
        const Vector2f& get_position() const;
        float get_rotation() const;
        const Vector2f& get_scale() const;
        const Vector2f& get_origin() const;
        void move(float offsetX, float offsetY);
        void move(const Vector2f& offset);
        void rotate(float angle);
        void scale(float factorX, float factorY);
        void scale(const Vector2f& factor);
        const Transform& get_transform() const;
        const Transform& get_inverse_transform() const;
        const void ResetTransform();

    private:

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        Vector2f          m_origin;                     //!< Origin of translation/rotation/scaling of the object
        Vector2f          m_position;                   //!< Position of the object in the 2D world
        float             m_rotation;                   //!< Orientation of the object, in degrees
        Vector2f          m_scale;                      //!< Scale of the object
        mutable Transform m_transform;                  //!< Combined transformation of the object
        mutable bool      m_transformNeedUpdate;        //!< Does the transform need to be recomputed?
        mutable Transform m_inverseTransform;           //!< Combined transformation of the object
        mutable bool      m_inverseTransformNeedUpdate; //!< Does the transform need to be recomputed?
    };

}

#endif