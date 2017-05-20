#include "Physics.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace physics {

inline Physics* Physics::Get()
{
    return Physics::instance_;
}

} // namespace physics
} // namespace engine