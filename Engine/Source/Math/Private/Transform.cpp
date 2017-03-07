#include "Math\Transform.h"

namespace engine {
namespace math {

    const Transform Transform::ZERO;

    Transform::Transform(Vec3D i_position, Vec3D i_rotation, Vec3D i_scale) : position_(i_position),
        rotation_(i_rotation),
        scale_(i_scale)
    {}

    Transform::Transform(const Transform& i_copy) : position_(i_copy.position_),
        rotation_(i_copy.rotation_),
        scale_(i_copy.scale_)
    {}

} // namespace math
} // namespace engine
