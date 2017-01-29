#include "Math\Vec3D.h"

// library includes
#include <cmath>

namespace engine {
namespace math {

	const Vec3D Vec3D::ZERO(0.0f, 0.0f, 0.0f);
	const Vec3D Vec3D::UNIT(1.0f, 1.0f, 1.0f);
	const Vec3D Vec3D::UNIT_X(1.0f, 0.0f, 0.0f);
	const Vec3D Vec3D::UNIT_Y(0.0f, 1.0f, 0.0f);
	const Vec3D Vec3D::UNIT_Z(0.0f, 0.0f, 1.0f);

	Vec3D::Vec3D(float i_x, float i_y, float i_z) : x_(i_x),
		y_(i_y),
		z_(i_z)
	{}

	Vec3D::Vec3D(const Vec3D& i_copy) : x_(i_copy.x_),
		y_(i_copy.y_),
		z_(i_copy.z_)
	{}

	float Vec3D::Length() const
	{
		return std::sqrtf(LengthSquared());
	}

	void Vec3D::Normalize()
	{
		float length_squared = x_ * x_ + y_ * y_ + z_ * z_;

		// return if already normalized
		if (FuzzyEquals(length_squared, 1.0f))
		{
			return;
		}

		float length = std::sqrtf(length_squared);
		length = 1.0f / length;

		x_ *= length;
		y_ *= length;
		z_ *= length;
	}

	Vec3D Vec3D::Normalize() const
	{
		Vec3D v(*this);
		v.Normalize();
		return v;
	}

} // namespace math
} // namespace engine