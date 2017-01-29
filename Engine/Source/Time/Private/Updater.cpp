#include "Time\Updater.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Time\InterfaceTickable.h"

namespace engine {
namespace time {

// static member initialization
Updater* Updater::instance_ = nullptr;

Updater::Updater() : num_tickables_(0)
{}

Updater::~Updater()
{}

Updater* Updater::Create()
{
	if (!Updater::instance_)
	{
		Updater::instance_ = new Updater();
	}
	return Updater::instance_;
}

void Updater::Destroy()
{
	SAFE_DELETE(Updater::instance_);
}

void Updater::Run(float dt)
{
	for (size_t i = 0; i < num_tickables_; ++i)
	{
		tickables_[i]->Update(dt);
	}
}

} // namespace time
} // namespace engin