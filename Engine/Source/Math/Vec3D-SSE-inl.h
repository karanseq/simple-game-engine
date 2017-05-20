#include "Vec3D-SSE.h"

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace math {
namespace optimized {

inline float Vec3D::x() const
{
    return x_;
}

inline void Vec3D::x(float i_x)
{
    ASSERT(!IsNaN(i_x));
    x_ = i_x;
}

inline float Vec3D::y() const
{
    return y_;
}

inline void Vec3D::y(float i_y)
{
    ASSERT(!IsNaN(i_y));
    y_ = i_y;
}

inline float Vec3D::z() const
{
    return z_;
}

inline void Vec3D::z(float i_z)
{
    ASSERT(!IsNaN(i_z));
    z_ = i_z;
}

inline void Vec3D::set(float i_x, float i_y, float i_z)
{
    ASSERT(!(IsNaN(i_x) || IsNaN(i_y) || IsNaN(i_z)));
    x_ = i_x;
    y_ = i_y;
    z_ = i_z;
}

inline void Vec3D::set(const __m128 i_vec)
{
    vec_ = i_vec;
}

inline Vec3D& Vec3D::operator=(const Vec3D& i_vec)
{
    // check for self assignment
    if (this != &i_vec)
    {
        x_ = i_vec.x_;
        y_ = i_vec.y_;
        z_ = i_vec.z_;
        vec_ = i_vec.vec_;
    }

    return *this;
}

inline Vec3D Vec3D::operator+(const Vec3D& i_vec) const
{
    return Vec3D(_mm_add_ps(vec_, i_vec.vec_));
}

inline Vec3D& Vec3D::operator+=(const Vec3D& i_vec)
{
    vec_ = _mm_add_ps(vec_, i_vec.vec_);
    return *this;
}

inline Vec3D Vec3D::operator-(const Vec3D& i_vec) const
{
    return Vec3D(_mm_sub_ps(vec_, i_vec.vec_));
}

inline Vec3D& Vec3D::operator-=(const Vec3D& i_vec)
{
    vec_ = _mm_sub_ps(vec_, i_vec.vec_);
    return *this;
}

inline Vec3D Vec3D::operator*(float i_scale) const
{
    ASSERT(!IsNaN(i_scale));
    return Vec3D(_mm_mul_ps(vec_, _mm_setr_ps(i_scale, i_scale, i_scale, 0.0f)));
}

inline Vec3D& Vec3D::operator*=(float i_scale)
{
    ASSERT(!IsNaN(i_scale));
    vec_ = _mm_mul_ps(vec_, _mm_setr_ps(i_scale, i_scale, i_scale, 0.0f));
    return *this;
}

inline bool Vec3D::operator==(const Vec3D& i_vec) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, i_vec.vec_)) == 0);
}

inline bool Vec3D::operator!=(const Vec3D& i_vec) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, i_vec.vec_)) != 0);
}

inline Vec3D Vec3D::operator-() const
{
    return Vec3D(_mm_sub_ps(_mm_set1_ps(0.0f), vec_));
}

inline bool Vec3D::IsZero() const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, _mm_set1_ps(0.0f))) != 0);
}

inline bool Vec3D::IsOne() const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, _mm_set1_ps(1.0f))) != 0);
}

inline float Vec3D::LengthSquared() const
{
    return (_mm_cvtss_f32(_mm_dp_ps(vec_, vec_, 0x71)));
}

} // namespace optimized
} // namespace math
} // namespace engine


