#ifndef ACTOR_H_
#define ACTOR_H_

// engine includes
#include "Data\HashedString.h"
#include "Data\PooledString.h"
#include "GameObject\GameObject.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"
#include "Physics\PhysicsObject.h"
#include "Renderer\RenderableObject.h"

namespace engine {
namespace gameobject {

class Actor
{
public:
    inline static engine::memory::SharedPointer<Actor> Create();
    inline static engine::memory::SharedPointer<Actor> Create(uint32_t i_id, const engine::data::PooledString& i_name, const engine::data::HashedString& i_type);
    inline static engine::memory::SharedPointer<Actor> Create(const engine::memory::SharedPointer<GameObject>& i_game_object);
    inline static engine::memory::SharedPointer<Actor> Create(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);
    inline static engine::memory::SharedPointer<Actor> Create(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object);
    inline static engine::memory::SharedPointer<Actor> Create(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object);
    ~Actor();

    // disable copy & move constructors & assignment operators
    Actor(const Actor& i_copy) = delete;
    Actor& operator=(const Actor& i_copy) = delete;
    Actor(Actor&& i_copy) = delete;
    Actor& operator=(Actor&& i_copy) = delete;

    // accessors and mutators
    inline void SetID(uint32_t i_id);
    inline uint32_t GetID() const;

    inline void SetName(const engine::data::PooledString& i_name);
    inline const engine::data::PooledString& GetName() const;

    inline void SetType(const engine::data::HashedString& i_type);
    inline const engine::data::HashedString& GetType() const;

    inline void SetGameObject(const engine::memory::SharedPointer<GameObject>& i_game_object);
    inline const engine::memory::SharedPointer<GameObject>& GetGameObject() const;

    inline void SetPhysicsObject(const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);
    inline const engine::memory::WeakPointer<engine::physics::PhysicsObject>& GetPhysicsObject() const;

    inline void SetRenderableObject(const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object);
    inline const engine::memory::WeakPointer<engine::render::RenderableObject>& GetRenderableObject() const;

    inline void SetHasDied(bool i_has_died);
    inline bool GetHasDied() const;

    inline void SetIsEnabled(bool i_is_enabled);
    inline bool GetIsEnabled() const;

protected:
    Actor();
    Actor(uint32_t i_id, const engine::data::PooledString& i_name, const engine::data::HashedString& i_type);
    Actor(const engine::memory::SharedPointer<GameObject>& i_game_object);
    Actor(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);
    Actor(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object);
    Actor(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object);

private:
    uint32_t                                                                    id_;
    engine::data::PooledString                                                  name_;
    engine::data::HashedString                                                  type_;
    engine::memory::SharedPointer<GameObject>                                   game_object_;
    engine::memory::WeakPointer<engine::physics::PhysicsObject>                 physics_object_;
    engine::memory::WeakPointer<engine::render::RenderableObject>               renderable_object_;
    bool                                                                        has_died_;
    bool                                                                        is_enabled_;

}; // class Actor

} // namespace gameobject
} // namespace engine

#include "Actor-inl.h"

#endif // ACTOR_H_
