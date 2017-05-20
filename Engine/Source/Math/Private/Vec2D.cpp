#include "Math\Vec2D.h"

// library includes
#include <cmath>

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace math {

    const Vec2D Vec2D::ZERO(0.0f, 0.0f);
    const Vec2D Vec2D::UNIT(1.0f, 1.0f);
    const Vec2D Vec2D::UNIT_X(1.0f, 0.0f);
    const Vec2D Vec2D::UNIT_Y(0.0f, 1.0f);

    Vec2D::Vec2D(float i_x, float i_y) : x_(i_x),
        y_(i_y)
    {
        ASSERT(!IsNaN(x_) && !IsNaN(y_));
    }

    Vec2D::Vec2D(const Vec2D& i_copy) : x_(i_copy.x_),
        y_(i_copy.y_)
    {}

    float Vec2D::Length() const
    {
        return std::sqrtf(LengthSquared());
    }

    void Vec2D::Normalize()
    {
        float length_squared = x_ * x_ + y_ * y_;

        // return if already normalized OR if length is zero
        if (FuzzyEquals(length_squared, 1.0f) || FuzzyEquals(length_squared, 0.0f))
        {
            return;
        }

        float length = std::sqrtf(length_squared);
        length = 1.0f / length;

        x_ *= length;
        y_ *= length;
    }

    Vec2D Vec2D::Normalize() const
    {
        Vec2D v(*this);
        v.Normalize();
        return v;
    }

} // namespace math
} // namespace engine