#include "Math\Size.h"

namespace engine {
namespace math {

	const Size Size::ZERO(0.0f, 0.0f);

	Size::Size(float width, float height) : width_(width),
		height_(height)
	{}

	Size::Size(const Size& i_copy) : width_(i_copy.width_),
		height_(i_copy.height_)
	{}

} // namespace math
} // namespace engine