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

	inline void Vec2D::x(float x)
	{
		ASSERT(!IsNaN(x));
		x_ = x;
	}

	inline float Vec2D::y() const
	{
		return y_;
	}

	inline void Vec2D::y(float y)
	{
		ASSERT(!IsNaN(y));
		y_ = y;
	}

	inline void Vec2D::set(float x, float y)
	{
		ASSERT(!IsNaN(x) && !IsNaN(y));
		x_ = x;
		y_ = y;
	}

	inline Vec2D& Vec2D::operator=(const Vec2D& vec)
	{
		// check for self assignment
		if (this != &vec)
		{
			x_ = vec.x_;
			y_ = vec.y_;
		}

		return *this;
	}

	inline Vec2D Vec2D::operator+(const Vec2D& vec) const
	{
		return Vec2D(x_ + vec.x(), y_ + vec.y());
	}

	inline Vec2D& Vec2D::operator+=(const Vec2D& vec)
	{
		x(x_ + vec.x());
		y(y_ + vec.y());
		return *this;
	}

	inline Vec2D Vec2D::operator-(const Vec2D& vec) const
	{
		return Vec2D(x_ - vec.x(), y_ - vec.y());
	}

	inline Vec2D& Vec2D::operator-=(const Vec2D& vec)
	{
		x(x_ - vec.x());
		y(y_ - vec.y());
		return *this;
	}

	inline Vec2D& Vec2D::operator*=(float scale)
	{
		ASSERT(!IsNaN(scale));
		x(x_ * scale);
		y(y_ * scale);
		return *this;
	}

	inline bool Vec2D::operator==(const Vec2D& vec) const
	{
		return (FuzzyEqual(x_, vec.x()) && FuzzyEqual(y_, vec.y()));
	}

	inline bool Vec2D::operator!=(const Vec2D& vec) const
	{
		return (x_ != vec.x() || y_ != vec.y());
	}

	inline Vec2D Vec2D::operator-() const
	{
		return Vec2D(-x(), -y());
	}

} // namespace math
} // namespace engine