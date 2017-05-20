#include "Math\Vec4D-SSE.h"

// engine includes
#include "Math\Vec3D-SSE.h"

namespace engine {
namespace math {
namespace optimized {

const Vec4D Vec4D::ZERO(_mm_set1_ps(0.0f));
const Vec4D Vec4D::UNIT(_mm_set1_ps(1.0f));
const Vec4D Vec4D::UNIT_X(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
const Vec4D Vec4D::UNIT_Y(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
const Vec4D Vec4D::UNIT_Z(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
const Vec4D Vec4D::UNIT_W(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));

Vec4D::Vec4D(float i_x, float i_y, float i_z, float i_w) : x_(i_x),
y_(i_y),
z_(i_z),
w_(i_w),
vec_(_mm_setr_ps(i_x, i_y, i_z, i_w))
{
    ASSERT(!IsNaN(i_x) && !IsNaN(i_y) && !IsNaN(i_z) && !IsNaN(i_w));
}

Vec4D::Vec4D(const __m128& i_vec) : vec_(i_vec)
{}

Vec4D::Vec4D(const Vec3D& i_vec3, float i_w) : x_(i_vec3.x()),
y_(i_vec3.y()),
z_(i_vec3.z()),
w_(i_w),
vec_(_mm_setr_ps(i_vec3.x(), i_vec3.y(), i_vec3.z(), i_w))
{}

Vec4D::Vec4D(const Vec4D& i_copy) : x_(i_copy.x_),
y_(i_copy.y_),
z_(i_copy.z_),
w_(i_copy.w_),
vec_(i_copy.vec_)
{}

float Vec4D::Length() const
{
    return (_mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(vec_, vec_, 0xf1))));
}

void Vec4D::Normalize()
{
    vec_ = _mm_mul_ps(vec_, _mm_rsqrt_ps(_mm_dp_ps(vec_, vec_, 0xff)));
}

Vec4D Vec4D::Normalize() const
{
    return Vec4D(_mm_mul_ps(vec_, _mm_rsqrt_ps(_mm_dp_ps(vec_, vec_, 0xff))));
}

} // namespace optimized
} // namespace math
} // namespace engine

