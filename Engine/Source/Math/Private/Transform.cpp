#include "Math\Transform.h"

namespace engine {
namespace math {

	Transform::Transform(Vec3D position, Vec3D rotation, Vec3D scale) : position_(position),
		rotation_(rotation),
		scale_(scale)
	{}

	Transform::Transform(const Transform& copy) : position_(copy.position_),
		rotation_(copy.rotation_),
		scale_(copy.scale_)
	{}

} // namespace math
} // namespace engine
