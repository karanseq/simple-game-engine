#include "Updater.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace time {

inline Updater* Updater::Get()
{
	return Updater::instance_;
}

inline void Updater::AddTickable(InterfaceTickable* i_tickable)
{
	// validate input
	ASSERT(i_tickable);

	// check if this object already exists
	if (std::find(tickables_.begin(), tickables_.end(), i_tickable) != tickables_.end())
	{
		LOG_ERROR("Updater is already tracking this tickable!");
		return;
	}

	tickables_.push_back(i_tickable);
	++num_tickables_;
}

inline void Updater::RemoveTickable(InterfaceTickable* i_tickable)
{
	// validate input
	ASSERT(i_tickable);
	// can't remove an object if there are none
	ASSERT(num_tickables_ > 0);

	// check if this object exists
	auto it = std::find(tickables_.begin(), tickables_.end(), i_tickable);
	if (it == tickables_.end())
	{
		LOG_ERROR("Updater could not find this tickable!");
		return;
	}

	tickables_.erase(it);
	--num_tickables_;
}

} // namespace time
} // namespace engine
