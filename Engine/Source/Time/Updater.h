#ifndef UPDATER_H_
#define UPDATER_H_

// library includes
#include <vector>

namespace engine {
namespace time {

// forward declarations
class InterfaceTickable;

/*
	Updater
	- A singleton that receives ticks from the engine and broadcasts the same to all objects that implement InterfaceTickable
	- Classes that wish to receive ticks must implement InterfaceTickable and add themselves to the Updater
	- Classes that wish to no longer receive ticks must remove themselves from the Updater
*/

class Updater
{
private:
	Updater();
	~Updater();
	static Updater* instance_;

	Updater(const Updater& i_copy) = delete;
	Updater& operator=(const Updater& i_copy) = delete;

public:
	static Updater* Create();
	static void Destroy();
	static inline Updater* Get();

	// add/remove a tickable
	inline void AddTickable(InterfaceTickable* i_tickable);
	inline void RemoveTickable(InterfaceTickable* i_tickable);

	// the tick
	void Run(float dt);

private:
	size_t													num_tickables_;
	std::vector<InterfaceTickable*>							tickables_;

}; // class Updater

} // namespace time
} // namespace engine

#include "Updater-inl.h"

#endif // UPDATER_H_