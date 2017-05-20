#include "Math\Vec3D-SSE.h"

// engine includes
#include "Math\Vec3D.h"

namespace engine {
namespace math {
namespace optimized {

const Vec3D Vec3D::ZERO(_mm_set1_ps(0.0f));
const Vec3D Vec3D::UNIT(_mm_set1_ps(1.0f));
const Vec3D Vec3D::UNIT_X(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
const Vec3D Vec3D::UNIT_Y(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
const Vec3D Vec3D::UNIT_Z(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));

Vec3D::Vec3D(float i_x, float i_y, float i_z) : x_(i_x),
y_(i_y),
z_(i_z),
vec_(_mm_setr_ps(i_x, i_y, i_z, 0.0f))
{
    ASSERT(!(IsNaN(i_x) || IsNaN(i_y) || IsNaN(i_z)));
}

Vec3D::Vec3D(const __m128 i_vec) : vec_(i_vec)
{}

Vec3D::Vec3D(const engine::math::Vec3D& i_copy) : x_(i_copy.x()),
    y_(i_copy.y()),
    z_(i_copy.z()),
    vec_(_mm_setr_ps(i_copy.x(), i_copy.y(), i_copy.z(), 0.0f))
{}

Vec3D::Vec3D(const Vec3D& i_copy) : x_(i_copy.x_),
y_(i_copy.y_),
z_(i_copy.z_),
vec_(i_copy.vec_)
{}

float Vec3D::Length() const
{
    return (_mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(vec_, vec_, 0x71))));
}

void Vec3D::Normalize()
{
    vec_ = _mm_mul_ps(vec_, _mm_rsqrt_ps(_mm_dp_ps(vec_, vec_, 0x77)));
}

Vec3D Vec3D::Normalize() const
{
    return Vec3D(_mm_mul_ps(vec_, _mm_rsqrt_ps(_mm_dp_ps(vec_, vec_, 0x77))));
}

} // namespace optimized
} // namespace math
} // namespace engine
