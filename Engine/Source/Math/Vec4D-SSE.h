#ifndef ENGINE_VEC4D_SSE_H_
#define ENGINE_VEC4D_SSE_H_

// library includes
#include <xmmintrin.h>
#include <smmintrin.h>

namespace engine {
namespace math {
namespace optimized {

// forward declarations
class Vec3D;

/*
Vec4D
- A class that represents a 3D point
- Overloads basic arithmetic operators, validates against bad floats
- Uses Intel's SSE via compiler intrinsics
*/
class Vec4D
{
public:
    explicit Vec4D(float i_x = 0.0f, float i_y = 0.0f, float i_z = 0.0f, float i_w = 0.0f);
    explicit Vec4D(const __m128& i_vec);
    explicit Vec4D(const Vec3D& i_vec3, float i_w = 0.0f);
    Vec4D(const Vec4D& i_copy);

    ~Vec4D()
    {}

    // accessors and mutators
    inline float x() const;
    inline void x(float i_x);
    inline float y() const;
    inline void y(float i_y);
    inline float z() const;
    inline void z(float i_z);
    inline float w() const;
    inline void w(float i_w);
    inline void set(float i_x = 0.0f, float i_y = 0.0f, float i_z = 0.0f, float i_w = 0.0f);
    inline void set(const __m128& i_vec);

    // assignment
    inline Vec4D& operator=(const Vec4D& i_vec);

    // arithmetic
    inline Vec4D operator+(const Vec4D& i_vec) const;
    inline Vec4D& operator+=(const Vec4D& i_vec);
    inline Vec4D operator-(const Vec4D& i_vec) const;
    inline Vec4D& operator-=(const Vec4D& i_vec);
    inline Vec4D operator*(float i_scale) const;
    inline Vec4D& operator*=(float i_scale);

    // relational
    inline bool operator==(const Vec4D& i_vec) const;
    inline bool operator!=(const Vec4D& i_vec) const;

    // unary
    inline Vec4D operator-() const;

    // functions
    inline bool IsZero() const;
    inline bool IsOne() const;
    inline bool IsPoint() const;
    inline float LengthSquared() const;
    float Length() const;
    void Normalize();
    Vec4D Normalize() const;

    // constants
    static const Vec4D          ZERO;
    static const Vec4D          UNIT;
    static const Vec4D          UNIT_X;
    static const Vec4D          UNIT_Y;
    static const Vec4D          UNIT_Z;
    static const Vec4D          UNIT_W;

protected:
    union
    {
        struct
        {
            float       x_;
            float       y_;
            float       z_;
            float       w_;
        };
        __m128          vec_;
    };

    friend class Mat44;

}; // class Vec4D

} // namespace optimized
} // namespace math
} // namespace engine

#include "Vec4D-SSE-inl.h"

#endif // ENGINE_VEC4D_SSE_H_
