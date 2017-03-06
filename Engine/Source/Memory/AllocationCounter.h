#ifdef BUILD_DEBUG

#ifndef ALLOCATION_COUNTER_H_
#define ALLOCATION_COUNTER_H_

// library includes
#include <mutex>
#include <vector>

#define COUNT_ALLOC(allocation_size) { if (engine::memory::AllocationCounter::Get()) { engine::memory::AllocationCounter::Get()->Add((allocation_size)); } }

namespace engine {
namespace memory {

class AllocationCount
{
public:
	AllocationCount();
	explicit AllocationCount(size_t i_allocation_size);
	~AllocationCount();

	inline bool operator<(const AllocationCount& i_other) const;
	inline bool operator==(const AllocationCount& i_other) const;
	inline AllocationCount& operator++();
	inline AllocationCount& operator++(int);
	inline AllocationCount& operator+=(size_t i_count);

private:
	size_t				allocation_;
	size_t				count_;

	friend class AllocationCounter;

}; // class AllocationCount

class AllocationCounter
{
public:
	static AllocationCounter* Create();
	static void Destroy();
	static inline AllocationCounter* Get();

	void Add(size_t i_alloc_size);
	void Dump();

	// constants
	static const size_t									MAX_ALLOCATION_SIZES_TO_TRACK;

private:
	AllocationCounter();
	~AllocationCounter() {}

	AllocationCounter(const AllocationCounter&) = delete;
	AllocationCounter(AllocationCounter&&) = delete;
	AllocationCounter& operator=(const AllocationCounter&) = delete;
	AllocationCounter& operator=(AllocationCounter&&) = delete;

	static AllocationCounter*							instance_;
	size_t												counter_size_;
	std::mutex											counter_mutex_;
	std::vector<AllocationCount>						counter_;

	friend class BlockAllocator;
	friend class FixedSizeAllocator;

}; // class AllocationCounter

} // namespace memory
} // namespace engine

#include "AllocationCounter-inl.h"

#endif // ALLOCATION_COUNTER_H_

#endif // BUILD_DEBUG
