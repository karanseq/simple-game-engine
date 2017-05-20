#include "Collider.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace physics {

inline Collider* Collider::Get()
{
    return Collider::instance_;
}

inline void Collider::SetCollisionListener(InterfaceCollisionListener* i_collision_listener)
{
    collision_listener_ = i_collision_listener;
}

} // namespace physics
} // namespace engine
