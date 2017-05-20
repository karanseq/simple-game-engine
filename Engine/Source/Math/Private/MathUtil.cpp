#include "Math\MathUtil.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\AABB.h"
#include "Math\Mat44.h"
#include "Math\Mat44-SSE.h"
#include "Math\Transform.h"
#include "Math\Vec2D.h"
#include "Math\Vec3D.h"
#include "Math\Vec3D-SSE.h"

namespace engine {
namespace math {

float DotProduct(const Vec2D& i_v1, const Vec2D& i_v2)
{
    return (i_v1.x() * i_v2.x() + i_v1.y() * i_v2.y());
}

float DotProduct(const Vec3D& i_v1, const Vec3D& i_v2)
{
    return (i_v1.x() * i_v2.x() + i_v1.y() * i_v2.y() + i_v1.z() * i_v2.z());
}

float DotProduct(const engine::math::optimized::Vec3D& i_v1, const engine::math::optimized::Vec3D& i_v2)
{
    return (_mm_cvtss_f32(_mm_dp_ps(i_v1.vec_, i_v2.vec_, 0x71)));
}

Vec3D CrossProduct(const Vec3D& i_v1, const Vec3D& i_v2)
{
    return Vec3D(i_v1.y() * i_v2.z() - i_v1.z() * i_v2.y(),
        i_v1.z() * i_v2.x() - i_v1.x() * i_v2.z(),
        i_v1.x() * i_v2.y() - i_v1.y() * i_v2.x());
}

engine::math::optimized::Vec3D CrossProduct(const engine::math::optimized::Vec3D& i_v1, const engine::math::optimized::Vec3D& i_v2)
{
    return engine::math::optimized::Vec3D(_mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(i_v1.vec_, i_v1.vec_, _MM_SHUFFLE(3, 0, 2, 1)),
            _mm_shuffle_ps(i_v2.vec_, i_v2.vec_, _MM_SHUFFLE(3, 1, 0, 2))),

        _mm_mul_ps(_mm_shuffle_ps(i_v1.vec_, i_v1.vec_, _MM_SHUFFLE(3, 1, 0, 2)),
            _mm_shuffle_ps(i_v2.vec_, i_v2.vec_, _MM_SHUFFLE(3, 0, 2, 1)))
    ));
}

void GetObjectToWorldTransform(const engine::math::Transform& i_transform, engine::math::Mat44& o_trans_mat)
{
    // get rotation matrix
    const engine::math::Mat44 mat_rot = engine::math::Mat44::GetRotationZ(i_transform.GetRotation().z());

    // get translation matrix
    const engine::math::Mat44 mat_trans = engine::math::Mat44::GetTranslation(i_transform.GetPosition());

    // get transform matrix
    o_trans_mat = mat_trans * mat_rot;
}

void GetObjectToWorldTransform(const engine::math::Transform& i_transform, engine::math::optimized::Mat44& o_trans_mat)
{
    // get rotation matrix
    const engine::math::optimized::Mat44 mat_rot = engine::math::optimized::Mat44::GetRotationZ(i_transform.GetRotation().z());

    // get translation matrix
    const engine::math::optimized::Mat44 mat_trans = engine::math::optimized::Mat44::GetTranslation(i_transform.GetPosition());

    // get transform matrix
    o_trans_mat = mat_trans * mat_rot;
}

} // namespace math
} // namespace engine