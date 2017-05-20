#include "Physics\Collider.h"

// engine includes
#include "Common\HelperMacros.h"
#include "GameObject\GameObject.h"
#include "Math\Mat44.h"
#include "Math\Mat44-SSE.h"
#include "Math\Vec3D-SSE.h"
#include "Math\Vec4D-SSE.h"
#include "Physics\PhysicsObject.h"
#include "Util\Profiler.h"

namespace engine {
namespace physics {

// static member initialization
Collider* Collider::instance_ = nullptr;

Collider::Collider() : num_dynamic_objects_(0),
    num_static_kynematic_objects_(0),
    collision_listener_(nullptr)
{}

Collider::~Collider()
{
    dynamic_objects_.clear();
    num_dynamic_objects_ = 0;
    static_kynematic_objects_.clear();
    num_static_kynematic_objects_ = 0;
}

Collider* Collider::Create()
{
    if (!Collider::instance_)
    {
        Collider::instance_ = new Collider();
    }
    return Collider::instance_;
}

void Collider::Destroy()
{
    SAFE_DELETE(Collider::instance_);
}

void Collider::Run(float i_dt)
{
    PROFILE_UNSCOPED("ColliderRun");

    PROFILE_SCOPE_BEGIN("CollisionDetection")
    DetectCollisions(i_dt);
    PROFILE_SCOPE_END
    RespondToCollisions(i_dt);
}

void Collider::DetectCollisions(float i_dt)
{
#ifdef ENABLE_FAST_MATH
    using namespace engine::math::optimized;
#else
    using namespace engine::math;
#endif

    // acquire a lock
    std::lock_guard<std::mutex> lock(collider_mutex_);

    for (size_t i = 0; i < num_dynamic_objects_; ++i)
    {
        // get physics object A
        const engine::memory::SharedPointer<PhysicsObject> physics_object_a = dynamic_objects_[i].Lock();
        if (!physics_object_a->GetIsActive())
        {
            continue;
        }

        // get physics object A's collision filter
        uint16_t collision_filter_a = physics_object_a->GetCollisionFilter();

        // get game object A
        const engine::memory::SharedPointer<engine::gameobject::GameObject> game_object_a = physics_object_a->GetGameObject().Lock();
        // get A's AABB
        const engine::math::AABB a_aabb = game_object_a->GetAABB();

        // calculate transform to convert from object A to world coordinates
        Mat44 mat_AtoW;
        engine::math::GetObjectToWorldTransform(game_object_a->GetTransform(), mat_AtoW);

        // calculate transform to convert world to object A coordinates
        Mat44 mat_WtoA(mat_AtoW.GetInverse());

        for (size_t j = i + 1, k = 0; j < num_dynamic_objects_ || k < num_static_kynematic_objects_; k += j < num_dynamic_objects_ ? 0 : 1, j += j < num_dynamic_objects_ ? 1 : 0)
        {
            // get physics object B
            engine::memory::SharedPointer<PhysicsObject> physics_object_b = j < num_dynamic_objects_ ? dynamic_objects_[j].Lock() : static_kynematic_objects_[k].Lock();
            if (!physics_object_b->GetIsActive())
            {
                continue;
            }

            // get physics object B's collision filter
            uint16_t collision_filter_b = physics_object_b->GetCollisionFilter();

            if (collision_filter_a != collision_filter_b)
            {
                continue;
            }

            // get game object B
            const engine::memory::SharedPointer<engine::gameobject::GameObject> game_object_b = physics_object_b->GetGameObject().Lock();
            // get B's AABB
            const engine::math::AABB b_aabb = game_object_b->GetAABB();

            // calculate transform to convert object B to world coordinates
            Mat44 mat_BtoW;
            engine::math::GetObjectToWorldTransform(game_object_b->GetTransform(), mat_BtoW);

            // calculate transform to convert world to object B coordinates
            Mat44 mat_WtoB(mat_BtoW.GetInverse());

            // calculate transform to convert from A to B coordinates
            Mat44 mat_AtoB = mat_WtoB * mat_AtoW;
            // calculate transform to convert from B to A coordinates
            Mat44 mat_BtoA(mat_AtoB.GetInverse());

            bool is_X_separated_in_B = false;
            bool is_Y_separated_in_B = false;

            float t_close_X_in_B = 0.0f;
            float t_open_X_in_B = 0.0f;
            float t_close_Y_in_B = 0.0f;
            float t_open_Y_in_B = 0.0f;

            // check for A in B's coordinate system
            {
                // calculate velocity of A relative to B
                const Vec3D relative_vel_AtoB = physics_object_a->GetVelocity() - physics_object_b->GetVelocity();

                // transform relative velocity (as a vector) to object B's coordinate system
                const Vec4D relative_vel_WtoB = mat_WtoB * Vec4D(relative_vel_AtoB, 0.0f);

                // transform A's AABB to B's coordinate system
                const Vec4D A_center_in_B = mat_AtoB * Vec4D(a_aabb.center, 1.0f);
                const Vec4D A_X_extent_in_B = mat_AtoB * Vec4D(a_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
                const Vec4D A_Y_extent_in_B = mat_AtoB * Vec4D(0.0f, a_aabb.extents.y(), 0.0f, 0.0f);
                const Vec4D A_extents_in_B(fabs(A_X_extent_in_B.x()) + fabs(A_Y_extent_in_B.x()), fabs(A_X_extent_in_B.y()) + fabs(A_Y_extent_in_B.y()), 0.0f, 0.0f);

                // for X-axis
                is_X_separated_in_B = CheckSeparationForAxis(relative_vel_WtoB.x(), b_aabb.center.x(), b_aabb.extents.x(), A_center_in_B.x(), A_extents_in_B.x(), i_dt, t_close_X_in_B, t_open_X_in_B);

                // for Y-axis
                is_Y_separated_in_B = CheckSeparationForAxis(relative_vel_WtoB.y(), b_aabb.center.y(), b_aabb.extents.y(), A_center_in_B.y(), A_extents_in_B.y(), i_dt, t_close_Y_in_B, t_open_Y_in_B);

            } // check for A in B's coordinate system    

            bool is_X_separated_in_A = false;
            bool is_Y_separated_in_A = false;

            float t_close_X_in_A = 0.0f;
            float t_open_X_in_A = 0.0f;
            float t_close_Y_in_A = 0.0f;
            float t_open_Y_in_A = 0.0f;

            // check for B in A's coordinate system
            {
                // calculate velocity of B relative to A
                const Vec3D relative_vel_BtoA = physics_object_b->GetVelocity() - physics_object_a->GetVelocity();

                // transform relative velocity (as a vector) to A's coordinate system
                const Vec4D relative_vel_WtoA = mat_WtoA * Vec4D(relative_vel_BtoA, 0.0f);

                // transform B's AABB to A's coordinate system
                const Vec4D B_center_in_A = mat_BtoA * Vec4D(b_aabb.center, 1.0f);
                const Vec4D B_X_extent_in_A = mat_BtoA * Vec4D(b_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
                const Vec4D B_Y_extent_in_A = mat_BtoA * Vec4D(0.0f, b_aabb.extents.y(), 0.0f, 0.0f);
                const Vec4D B_extents_in_A(fabs(B_X_extent_in_A.x()) + fabs(B_Y_extent_in_A.x()), fabs(B_X_extent_in_A.y()) + fabs(B_Y_extent_in_A.y()), 0.0f, 0.0f);

                // for X-axis
                is_X_separated_in_A = CheckSeparationForAxis(relative_vel_WtoA.x(), a_aabb.center.x(), a_aabb.extents.x(), B_center_in_A.x(), B_extents_in_A.x(), i_dt, t_close_X_in_A, t_open_X_in_A);

                // for Y-axis
                is_Y_separated_in_A = CheckSeparationForAxis(relative_vel_WtoA.y(), a_aabb.center.y(), a_aabb.extents.y(), B_center_in_A.y(), B_extents_in_A.y(), i_dt, t_close_Y_in_A, t_open_Y_in_A);

            } // check for B in A's coordinate system

            // was there an obvious separation?
            if (!(is_X_separated_in_B || is_Y_separated_in_B || is_X_separated_in_A || is_Y_separated_in_A))
            {
                // find the latest t_close and the earliest t_open
                float t_close_latest = engine::math::GetMaxOfFour(t_close_X_in_B, t_close_Y_in_B, t_close_X_in_A, t_close_Y_in_A);
                float t_open_earliest = engine::math::GetMinOfFour(t_open_X_in_B, t_open_Y_in_B, t_open_X_in_A, t_open_Y_in_A);

                // if the latest t_close was after the earliest t_open, there was continuity of separation
                if (t_close_latest > t_open_earliest)
                {
                    //VERBOSE("Collision not found!");
                }
                else
                {
                    // calculate the normal to the surface that collided
                    Vec3D normal = Vec3D::ZERO;
                    if (engine::math::FuzzyEquals(t_close_latest, t_close_X_in_B))
                    {
                        const Vec4D B_X_in_W = mat_BtoW * Vec4D(1.0f, 0.0f, 0.0f, 0.0f);
                        normal.set(B_X_in_W.x(), B_X_in_W.y(), B_X_in_W.z());
                    }
                    else if (engine::math::FuzzyEquals(t_close_latest, t_close_Y_in_B))
                    {
                        const Vec4D B_Y_in_W = mat_BtoW * Vec4D(0.0f, 1.0f, 0.0f, 0.0f);
                        normal.set(B_Y_in_W.x(), B_Y_in_W.y(), B_Y_in_W.z());
                    }
                    else if (engine::math::FuzzyEquals(t_close_latest, t_close_X_in_A))
                    {
                        const Vec4D A_X_in_W = mat_AtoW * Vec4D(1.0f, 0.0f, 0.0f, 0.0f);
                        normal.set(A_X_in_W.x(), A_X_in_W.y(), A_X_in_W.z());
                    }
                    else if (engine::math::FuzzyEquals(t_close_latest, t_close_Y_in_A))
                    {
                        const Vec4D A_Y_in_W = mat_AtoW * Vec4D(0.0f, 1.0f, 0.0f, 0.0f);
                        normal.set(A_Y_in_W.x(), A_Y_in_W.y(), A_Y_in_W.z());
                    }

                    collided_objects_.push_back({t_close_latest, normal, physics_object_a, physics_object_b});
                }
            }

        } // end of inner for loop

    } // end of outer for loop

}

bool Collider::CheckSeparationForAxis(const float i_relative_vel_WtoA, const float i_a_aabb_center, const float i_a_aabb_extents, const float i_B_center_in_A, const float i_B_extents_in_a, const float i_dt, float &o_t_close, float &o_t_open)
{
    // treat zero velocities differently
    if (engine::math::FuzzyEquals(i_relative_vel_WtoA, 0.0f))
    {
        // separation check without velocities
        return (fabs(i_a_aabb_center - i_B_center_in_A) > i_a_aabb_extents + i_B_extents_in_a);
    }
    else
    {
        // calculate separation close and open times
        o_t_close = (i_a_aabb_center - i_a_aabb_extents - i_B_center_in_A - i_B_extents_in_a) / i_relative_vel_WtoA;
        o_t_open = (i_a_aabb_center + i_a_aabb_extents - i_B_center_in_A + i_B_extents_in_a) / i_relative_vel_WtoA;

        // t_close must be less than t_open
        // if not, swap them
        if (o_t_open < o_t_close)
        {
            float t_swap = o_t_open;
            o_t_open = o_t_close;
            o_t_close = t_swap;
        }

        // if t_open < 0, the separation occurred in the past
        // if t_close > i_dt, the separation will occur in the future
        return (o_t_open < 0 || o_t_close > i_dt);
    }
}

void Collider::RespondToCollisions(float i_dt)
{
    const size_t num_collided_objects = collided_objects_.size();

    for (size_t i = 0; i < num_collided_objects; ++i)
    {
        CollisionPair& collision_pair = collided_objects_[i];
        collision_pair.object_a.Lock()->RespondToCollision(collision_pair.normal);
        collision_pair.object_b.Lock()->RespondToCollision(collision_pair.normal);

        if (collision_listener_)
        {
            collision_listener_->OnCollision(collision_pair);
        }
    }

    collided_objects_.clear();
}

#ifdef BUILD_DEBUG
void Collider::PrintDebugInformation(const engine::math::Mat44& i_mat_WtoA,
    const engine::math::Mat44& i_mat_WtoB, 
    const engine::math::Mat44& i_mat_AtoB,
    const engine::math::Mat44& i_mat_BtoA, 
    const engine::math::AABB& i_a_aabb, 
    const engine::math::AABB& i_b_aabb, 
    const engine::math::Vec3D& i_a_vel, 
    const engine::math::Vec3D& i_b_vel,
    float i_dt) const
{
    LOG("******************************");

    const engine::math::Vec4D A_center_in_B = i_mat_AtoB * engine::math::Vec4D(i_a_aabb.center, 1.0f);
    const engine::math::Vec4D A_X_extent_in_B = i_mat_AtoB * engine::math::Vec4D(i_a_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
    const engine::math::Vec4D A_Y_extent_in_B = i_mat_AtoB * engine::math::Vec4D(0.0f, i_a_aabb.extents.y(), 0.0f, 0.0f);
    const engine::math::Vec4D A_extents_in_B = A_X_extent_in_B + A_Y_extent_in_B;

    const engine::math::Vec4D B_center_in_A = i_mat_BtoA * engine::math::Vec4D(i_b_aabb.center, 1.0f);
    const engine::math::Vec4D B_X_extent_in_A = i_mat_BtoA * engine::math::Vec4D(i_b_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
    const engine::math::Vec4D B_Y_extent_in_A = i_mat_BtoA * engine::math::Vec4D(0.0f, i_b_aabb.extents.y(), 0.0f, 0.0f);
    const engine::math::Vec4D B_extents_in_A = B_X_extent_in_A + B_Y_extent_in_A;

    const engine::math::Vec4D relative_vel_in_A = i_mat_WtoA * engine::math::Vec4D(i_a_vel - i_b_vel, 0.0f);
    const engine::math::Vec4D relative_vel_in_B = i_mat_WtoB * engine::math::Vec4D(i_b_vel - i_a_vel, 0.0f);

    LOG("A_to_B:");
    i_mat_AtoB.Print();
    LOG("B_to_A:");
    i_mat_BtoA.Print();

    LOG("A_center: %f, %f", i_a_aabb.center.x(), i_a_aabb.center.y());
    LOG("A_center_in_B: %f, %f", A_center_in_B.x(), A_center_in_B.y());
    LOG("A_extents: %f, %f", i_a_aabb.extents.x(), i_a_aabb.extents.y());
    LOG("A_extents_in_B: %f, %f", A_extents_in_B.x(), A_extents_in_B.y());

    LOG("B_center: %f, %f", i_b_aabb.center.x(), i_b_aabb.center.y());
    LOG("B_center_in_A: %f, %f", B_center_in_A.x(), B_center_in_A.y());
    LOG("B_extents: %f, %f", i_b_aabb.extents.x(), i_b_aabb.extents.y());
    LOG("B_extents_in_A: %f, %f", B_extents_in_A.x(), B_extents_in_A.y());

    LOG("******************************\n");
}
#endif // BUILD_DEBUG

void Collider::AddPhysicsObject(const engine::memory::WeakPointer<PhysicsObject>& i_physics_object)
{
    // validate input
    ASSERT(i_physics_object);

    // acquire a lock
    std::lock_guard<std::mutex> lock(collider_mutex_);

    if (i_physics_object.Lock()->GetType() == PhysicsObjectType::kPhysicsObjectDynamic)
    {
        // check if this object already exists
        if (std::find(dynamic_objects_.begin(), dynamic_objects_.end(), i_physics_object) != dynamic_objects_.end())
        {
            LOG_ERROR("Collider is already tracking this physics object!");
            return;
        }
    
        // add it to the list
        dynamic_objects_.push_back(i_physics_object);
        ++num_dynamic_objects_;
    }
    else
    {
        // check if this object already exists
        if (std::find(static_kynematic_objects_.begin(), static_kynematic_objects_.end(), i_physics_object) != static_kynematic_objects_.end())
        {
            LOG_ERROR("Collider is already tracking this physics object!");
            return;
        }

        // add it to the list
        static_kynematic_objects_.push_back(i_physics_object);
        ++num_static_kynematic_objects_;
    }
}

void Collider::RemovePhysicsObject(const engine::memory::WeakPointer<PhysicsObject>& i_physics_object)
{
    // validate input
    ASSERT(i_physics_object);
    // can't remove an object if there are none
    ASSERT(num_dynamic_objects_ > 0 || num_static_kynematic_objects_ > 0);

    // acquire a lock
    std::lock_guard<std::mutex> lock(collider_mutex_);

    if (i_physics_object.Lock()->GetType() == PhysicsObjectType::kPhysicsObjectDynamic)
    {
        // check if this object exists
        auto it = std::find(dynamic_objects_.begin(), dynamic_objects_.end(), i_physics_object);
        if (it == dynamic_objects_.end())
        {
            LOG_ERROR("Physics could not find this physics object!");
            return;
        }

        // remove it from the list
        dynamic_objects_.erase(it);
        --num_dynamic_objects_;
    }
    else
    {
        // check if this object exists
        auto it = std::find(static_kynematic_objects_.begin(), static_kynematic_objects_.end(), i_physics_object);
        if (it == static_kynematic_objects_.end())
        {
            LOG_ERROR("Physics could not find this physics object!");
            return;
        }

        // remove it from the list
        static_kynematic_objects_.erase(it);
        --num_static_kynematic_objects_;
    }
}

} // namespace physics
} // namespace engine