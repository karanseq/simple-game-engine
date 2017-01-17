#include "Math\Vec3D.h"

namespace engine {
namespace math {

	const Vec3D Vec3D::ZERO(0.0f, 0.0f, 0.0f);
	const Vec3D Vec3D::UNIT(1.0f, 1.0f, 1.0f);
	const Vec3D Vec3D::UNIT_X(1.0f, 0.0f, 0.0f);
	const Vec3D Vec3D::UNIT_Y(0.0f, 1.0f, 0.0f);
	const Vec3D Vec3D::UNIT_Z(0.0f, 0.0f, 1.0f);

	Vec3D::Vec3D(float x, float y, float z) : x_(x),
		y_(y),
		z_(z)
	{}

	Vec3D::Vec3D(const Vec3D& copy) : x_(copy.x_),
		y_(copy.y_),
		z_(copy.z_)
	{}

} // namespace math
} // namespace engine