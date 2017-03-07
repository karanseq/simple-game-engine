#ifndef ENGINE_GAME_OBJECT_CONTROLLER_H_
#define ENGINE_GAME_OBJECT_CONTROLLER_H_

// engine includes
#include "Memory\SharedPointer.h"

namespace engine {
namespace gameobject {

// forward declaration
class GameObject;

/*
    InterfaceGameObjectController
    - An interface that offers handles to control a GameObject.
*/
class InterfaceGameObjectController
{
public:
    virtual ~InterfaceGameObjectController() {}

    virtual InterfaceGameObjectController* Clone() const = 0;

    virtual engine::memory::SharedPointer<GameObject> GetGameObject() const = 0;
    virtual void SetGameObject(const engine::memory::SharedPointer<engine::gameobject::GameObject>& i_game_object) = 0;

    virtual void UpdateGameObject() = 0;
}; // class InterfaceGameObjectController

} // namespace gameobject
} // namespace engine

#endif // ENGINE_GAME_OBJECT_CONTROLLER_H_
