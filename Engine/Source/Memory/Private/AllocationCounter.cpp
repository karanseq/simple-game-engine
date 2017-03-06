#ifdef BUILD_DEBUG

#include "Memory\AllocationCounter.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Common\HelperMacros.h"
#include "Logger\Logger.h"

namespace engine {
namespace memory {

/********** AllocationCount START **********/
AllocationCount::AllocationCount() : allocation_(0),
	count_(0)
{}

AllocationCount::AllocationCount(size_t i_allocation_size) : allocation_(i_allocation_size),
	count_(1)
{}

AllocationCount::~AllocationCount()
{}

/********** AllocationCount END **********/

/********** AllocationCounter START **********/
const size_t AllocationCounter::MAX_ALLOCATION_SIZES_TO_TRACK = 1024;
AllocationCounter* AllocationCounter::instance_ = nullptr;

AllocationCounter* AllocationCounter::Create()
{
	if (AllocationCounter::instance_ == nullptr)
	{
		AllocationCounter::instance_ = new AllocationCounter();
	}
	return AllocationCounter::instance_;
}

void AllocationCounter::Destroy()
{
	SAFE_DELETE(AllocationCounter::instance_);
}

AllocationCounter::AllocationCounter() : counter_size_(0)
{
	counter_.reserve(AllocationCounter::MAX_ALLOCATION_SIZES_TO_TRACK);
}

void AllocationCounter::Add(size_t i_alloc_size)
{
	// validate state and input
	ASSERT(counter_size_ < MAX_ALLOCATION_SIZES_TO_TRACK);
	ASSERT(i_alloc_size > 0);

	std::lock_guard<std::mutex> lock(counter_mutex_);

	std::vector<AllocationCount>::iterator it = std::find(counter_.begin(), counter_.end(), AllocationCount(i_alloc_size));
	if (it == counter_.end())
	{
		counter_.push_back(AllocationCount(i_alloc_size));
		++counter_size_;
	}
	else
	{
		(*it)++;
	}
}

void AllocationCounter::Dump()
{
	std::lock_guard<std::mutex> lock(counter_mutex_);

	LOG("---------- %s ----------", __FUNCTION__);
	LOG("Dumping memory usage statistics:");
	std::sort(counter_.begin(), counter_.end());
	for (auto i : counter_)
	{
		LOG("%zu bytes:%zu allocations", i.allocation_, i.count_);
	}
	LOG("---------- END ----------");
}

/********** AllocationCounter END **********/

} // namespace memory
} // namespace engine

#endif // BUILD_DEBUG