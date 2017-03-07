#include "Vec2D.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace math {

    inline float Vec2D::x() const
    {
        return x_;
    }

    inline void Vec2D::x(float i_x)
    {
        ASSERT(!IsNaN(i_x));
        x_ = i_x;
    }

    inline float Vec2D::y() const
    {
        return y_;
    }

    inline void Vec2D::y(float i_y)
    {
        ASSERT(!IsNaN(i_y));
        y_ = i_y;
    }

    inline void Vec2D::set(float i_x, float i_y)
    {
        ASSERT(!IsNaN(i_x) && !IsNaN(i_y));
        x_ = i_x;
        y_ = i_y;
    }

    inline Vec2D& Vec2D::operator=(const Vec2D& i_vec)
    {
        // check for self assignment
        if (this != &i_vec)
        {
            x_ = i_vec.x_;
            y_ = i_vec.y_;
        }

        return *this;
    }

    inline Vec2D Vec2D::operator+(const Vec2D& i_vec) const
    {
        return Vec2D(x_ + i_vec.x(), y_ + i_vec.y());
    }

    inline Vec2D& Vec2D::operator+=(const Vec2D& i_vec)
    {
        x(x_ + i_vec.x());
        y(y_ + i_vec.y());
        return *this;
    }

    inline Vec2D Vec2D::operator-(const Vec2D& i_vec) const
    {
        return Vec2D(x_ - i_vec.x(), y_ - i_vec.y());
    }

    inline Vec2D& Vec2D::operator-=(const Vec2D& i_vec)
    {
        x(x_ - i_vec.x());
        y(y_ - i_vec.y());
        return *this;
    }

    inline Vec2D Vec2D::operator*(float i_scale) const
    {
        ASSERT(!IsNaN(i_scale));
        return Vec2D(x_ * i_scale, y_ * i_scale);
    }

    inline Vec2D& Vec2D::operator*=(float i_scale)
    {
        ASSERT(!IsNaN(i_scale));
        x(x_ * i_scale);
        y(y_ * i_scale);
        return *this;
    }

    inline bool Vec2D::operator==(const Vec2D& i_vec) const
    {
        return (FuzzyEquals(x_, i_vec.x()) && FuzzyEquals(y_, i_vec.y()));
    }

    inline bool Vec2D::operator!=(const Vec2D& i_vec) const
    {
        return (x_ != i_vec.x() || y_ != i_vec.y());
    }

    inline Vec2D Vec2D::operator-() const
    {
        return Vec2D(-x(), -y());
    }

    inline bool Vec2D::IsZero() const
    {
        return (FuzzyEquals(x_, 0.0f) && FuzzyEquals(y_, 0.0f));
    }

    inline bool Vec2D::IsOne() const
    {
        return (FuzzyEquals(x_, 1.0f) && FuzzyEquals(y_, 1.0f));
    }

    inline float Vec2D::LengthSquared() const
    {
        return (x_ * x_ + y_ * y_);
    }

} // namespace math
} // namespace engine