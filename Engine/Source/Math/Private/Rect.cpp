#include "Math\Rect.h"

namespace engine {
namespace math {

	const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

	Rect::Rect(float x, float y, float width, float height) : origin_(x, y),
		size_(width, height)
	{}

	Rect::Rect(const Vec2D& i_origin, const Size& i_size) : origin_(i_origin),
		size_(i_size)
	{}

	Rect::Rect(const Rect& i_copy) : origin_(i_copy.origin_),
		size_(i_copy.size_)
	{}

} // namespace math
} // namespace engine