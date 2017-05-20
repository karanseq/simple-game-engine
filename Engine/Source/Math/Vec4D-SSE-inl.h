#include "Vec4D.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace math {
namespace optimized {

inline float Vec4D::x() const
{
    return x_;
}

inline void Vec4D::x(float i_x)
{
    ASSERT(!IsNaN(i_x));
    x_ = i_x;
}

inline float Vec4D::y() const
{
    return y_;
}

inline void Vec4D::y(float i_y)
{
    ASSERT(!IsNaN(i_y));
    y_ = i_y;
}

inline float Vec4D::z() const
{
    return z_;
}

inline void Vec4D::z(float i_z)
{
    ASSERT(!IsNaN(i_z));
    z_ = i_z;
}

inline float Vec4D::w() const
{
    return w_;
}

inline void Vec4D::w(float i_w)
{
    ASSERT(!IsNaN(i_w));
    w_ = i_w;
}

inline void Vec4D::set(float i_x, float i_y, float i_z, float i_w)
{
    ASSERT(!IsNaN(i_x) && !IsNaN(i_y) && !IsNaN(i_z) && !IsNaN(i_w));
    x_ = i_x;
    y_ = i_y;
    z_ = i_z;
    w_ = i_w;
}

inline void Vec4D::set(const __m128& i_vec)
{
    vec_ = i_vec;
}

inline Vec4D& Vec4D::operator=(const Vec4D& i_vec)
{
    // check for self assignment
    if (this != &i_vec)
    {
        x_ = i_vec.x_;
        y_ = i_vec.y_;
        z_ = i_vec.z_;
        w_ = i_vec.w_;
        vec_ = i_vec.vec_;
    }

    return *this;
}

inline Vec4D Vec4D::operator+(const Vec4D& i_vec) const
{
    return Vec4D(_mm_add_ps(vec_, i_vec.vec_));
}

inline Vec4D& Vec4D::operator+=(const Vec4D& i_vec)
{
    vec_ = _mm_add_ps(vec_, i_vec.vec_);
    return *this;
}

inline Vec4D Vec4D::operator-(const Vec4D& i_vec) const
{
    return Vec4D(_mm_sub_ps(vec_, i_vec.vec_));
}

inline Vec4D& Vec4D::operator-=(const Vec4D& i_vec)
{
    vec_ = _mm_sub_ps(vec_, i_vec.vec_);
    return *this;
}

inline Vec4D Vec4D::operator*(float i_scale) const
{
    ASSERT(!IsNaN(i_scale));
    return Vec4D(_mm_mul_ps(vec_, _mm_setr_ps(i_scale, i_scale, i_scale, i_scale)));
}

inline Vec4D& Vec4D::operator*=(float i_scale)
{
    ASSERT(!IsNaN(i_scale));
    vec_ = _mm_mul_ps(vec_, _mm_setr_ps(i_scale, i_scale, i_scale, i_scale));
    return *this;
}

inline bool Vec4D::operator==(const Vec4D& i_vec) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, i_vec.vec_)) == 0);
}

inline bool Vec4D::operator!=(const Vec4D& i_vec) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, i_vec.vec_)) != 0);
}

inline Vec4D Vec4D::operator-() const
{
    return Vec4D(_mm_sub_ps(_mm_set1_ps(0.0f), vec_));
}

inline bool Vec4D::IsZero() const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, _mm_set1_ps(0.0f))) != 0);
}

inline bool Vec4D::IsOne() const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(vec_, _mm_set1_ps(1.0f))) != 0);
}

inline bool Vec4D::IsPoint() const
{
    return !(FuzzyEquals(w_, 0.0f));
}

inline float Vec4D::LengthSquared() const
{
    return (_mm_cvtss_f32(_mm_dp_ps(vec_, vec_, 0xf1)));
}

} // namespace optimized
} // namespace math
} // namespace engine

