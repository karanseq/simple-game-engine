#include "Vec4D.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace math {

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

    inline Vec4D& Vec4D::operator=(const Vec4D& i_vec)
    {
        // check for self assignment
        if (this != &i_vec)
        {
            x_ = i_vec.x_;
            y_ = i_vec.y_;
            z_ = i_vec.z_;
            w_ = i_vec.w_;
        }

        return *this;
    }

    inline Vec4D Vec4D::operator+(const Vec4D& i_vec) const
    {
        return Vec4D(x_ + i_vec.x(), y_ + i_vec.y(), z_ + i_vec.z(), w_ + i_vec.w());
    }

    inline Vec4D& Vec4D::operator+=(const Vec4D& i_vec)
    {
        x(x_ + i_vec.x());
        y(y_ + i_vec.y());
        z(z_ + i_vec.z());
        w(w_ + i_vec.w());
        return *this;
    }

    inline Vec4D Vec4D::operator-(const Vec4D& i_vec) const
    {
        return Vec4D(x_ - i_vec.x(), y_ - i_vec.y(), z_ - i_vec.z(), w_ - i_vec.w());
    }

    inline Vec4D& Vec4D::operator-=(const Vec4D& i_vec)
    {
        x(x_ - i_vec.x());
        y(y_ - i_vec.y());
        z(z_ - i_vec.z());
        w(w_ - i_vec.w());
        return *this;
    }

    inline Vec4D Vec4D::operator*(float i_scale) const
    {
        ASSERT(!IsNaN(i_scale));
        return Vec4D(x_ * i_scale, y_ * i_scale, z_ * i_scale, w_ * i_scale);
    }

    inline Vec4D& Vec4D::operator*=(float i_scale)
    {
        ASSERT(!IsNaN(i_scale));
        x(x_ * i_scale);
        y(y_ * i_scale);
        z(z_ * i_scale);
        w(w_ * i_scale);
        return *this;
    }

    inline bool Vec4D::operator==(const Vec4D& i_vec) const
    {
        return (FuzzyEquals(x_, i_vec.x()) && FuzzyEquals(y_, i_vec.y()) && FuzzyEquals(z_, i_vec.z()) && FuzzyEquals(w_, i_vec.w()));
    }

    inline bool Vec4D::operator!=(const Vec4D& i_vec) const
    {
        return (x_ != i_vec.x() || y_ != i_vec.y() || z_ != i_vec.z() || w_ != i_vec.w());
    }

    inline Vec4D Vec4D::operator-() const
    {
        return Vec4D(-x(), -y(), -z(), -w());
    }
    
    inline bool Vec4D::IsZero() const
    {
        return (FuzzyEquals(x_, 0.0f) && FuzzyEquals(y_, 0.0f) && FuzzyEquals(z_, 0.0f) && FuzzyEquals(w_, 0.0f));
    }

    inline bool Vec4D::IsOne() const
    {
        return (FuzzyEquals(x_, 1.0f) && FuzzyEquals(y_, 1.0f) && FuzzyEquals(z_, 1.0f) && FuzzyEquals(w_, 1.0f));
    }

    inline bool Vec4D::IsPoint() const
    {
        return !(FuzzyEquals(w_, 0.0f));
    }

    inline float Vec4D::LengthSquared() const
    {
        return (x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
    }

} // namespace math
} // namespace engine
