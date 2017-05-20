#ifndef AABB_H_
#define AABB_H_

// engine includes
#include "Vec3D.h"

namespace engine {
namespace math {

struct AABB
{
    Vec3D                   center;
    Vec3D                   extents;

    static const AABB       ZERO;
};

} // namespace math
} // namespace engine

#endif // AABB_H_