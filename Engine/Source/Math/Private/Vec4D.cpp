#include "Math\Vec4D.h"

// library includes
#include <cmath>

namespace engine {
namespace math {

const Vec4D Vec4D::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Vec4D Vec4D::UNIT(1.0f, 1.0f, 1.0f, 1.0f);
const Vec4D Vec4D::UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
const Vec4D Vec4D::UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
const Vec4D Vec4D::UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
const Vec4D Vec4D::UNIT_W(0.0f, 0.0f, 0.0f, 1.0f);

Vec4D::Vec4D(float i_x, float i_y, float i_z, float i_w) : x_(i_x),
    y_(i_y),
    z_(i_z),
    w_(i_w)
{
    ASSERT(!IsNaN(x_) && !IsNaN(y_) && !IsNaN(z_) && !IsNaN(w_));
}

Vec4D::Vec4D(const Vec3D& i_vec3, float i_w) : x_(i_vec3.x()),
    y_(i_vec3.y()),
    z_(i_vec3.z()),
    w_(i_w)
{}

Vec4D::Vec4D(const Vec4D& i_copy) : x_(i_copy.x_),
    y_(i_copy.y_),
    z_(i_copy.z_),
    w_(i_copy.w_)
{}

float Vec4D::Length() const
{
    return std::sqrtf(LengthSquared());
}

void Vec4D::Normalize()
{
    float length_squared = x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;

    // return if already normalized OR if length is zero
    if (FuzzyEquals(length_squared, 1.0f) || FuzzyEquals(length_squared, 0.0f))
    {
        return;
    }

    float length = std::sqrtf(length_squared);
    length = 1.0f / length;

    x_ *= length;
    y_ *= length;
    z_ *= length;
    w_ *= length;
}

Vec4D Vec4D::Normalize() const
{
    Vec4D v(*this);
    v.Normalize();
    return v;
}

} // namespace math
} // namespace engine