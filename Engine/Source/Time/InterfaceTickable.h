#ifndef INTERFACE_TICKABLE_H_
#define INTERFACE_TICKABLE_H_

namespace engine {
namespace time {

/*
	InterfaceTickable
	- An interface that allows the implementers to receive ticks from the engine.
*/

class InterfaceTickable
{
public:
	virtual ~InterfaceTickable() {}

	virtual void Update(float dt) = 0;

}; // class InterfaceTickable

} // namespace time
} // namespace engine

#endif // INTERFACE_TICKABLE_H_
