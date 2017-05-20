#include "Vec3D.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace math {

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
    ASSERT(!IsNaN(i_x) && !IsNaN(i_y) && !IsNaN(i_z));
    x_ = i_x;
    y_ = i_y;
    z_ = i_z;
}

inline Vec3D& Vec3D::operator=(const Vec3D& i_vec)
{
    // check for self assignment
    if (this != &i_vec)
    {
        x_ = i_vec.x_;
        y_ = i_vec.y_;
        z_ = i_vec.z_;
    }

    return *this;
}

inline Vec3D Vec3D::operator+(const Vec3D& i_vec) const
{
    return Vec3D(x_ + i_vec.x(), y_ + i_vec.y(), z_ + i_vec.z());
}

inline Vec3D& Vec3D::operator+=(const Vec3D& i_vec)
{
    x(x_ + i_vec.x());
    y(y_ + i_vec.y());
    z(z_ + i_vec.z());
    return *this;
}

inline Vec3D Vec3D::operator-(const Vec3D& i_vec) const
{
    return Vec3D(x_ - i_vec.x(), y_ - i_vec.y(), z_ - i_vec.z());
}

inline Vec3D& Vec3D::operator-=(const Vec3D& i_vec)
{
    x(x_ - i_vec.x());
    y(y_ - i_vec.y());
    z(z_ - i_vec.z());
    return *this;
}

inline Vec3D Vec3D::operator*(float i_scale) const
{
    ASSERT(!IsNaN(i_scale));
    return Vec3D(x_ * i_scale, y_ * i_scale, z_ * i_scale);
}

inline Vec3D& Vec3D::operator*=(float i_scale)
{
    ASSERT(!IsNaN(i_scale));
    x(x_ * i_scale);
    y(y_ * i_scale);
    z(z_ * i_scale);
    return *this;
}

inline bool Vec3D::operator==(const Vec3D& i_vec) const
{
    return (FuzzyEquals(x_, i_vec.x()) && FuzzyEquals(y_, i_vec.y()) && FuzzyEquals(z_, i_vec.z()));
}

inline bool Vec3D::operator!=(const Vec3D& i_vec) const
{
    return (x_ != i_vec.x() || y_ != i_vec.y() || z_ != i_vec.z());
}

inline Vec3D Vec3D::operator-() const
{
    return Vec3D(-x(), -y(), -z());
}
    
inline bool Vec3D::IsZero() const
{
    return (FuzzyEquals(x_, 0.0f) && FuzzyEquals(y_, 0.0f) && FuzzyEquals(z_, 0.0f));
}

inline bool Vec3D::IsOne() const
{
    return (FuzzyEquals(x_, 1.0f) && FuzzyEquals(y_, 1.0f) && FuzzyEquals(z_, 1.0f));
}

inline float Vec3D::LengthSquared() const
{
    return (x_ * x_ + y_ * y_ + z_ * z_);
}

} // namespace math
} // namespace engine
