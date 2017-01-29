#ifndef ENGINE_FIXED_SIZE_ALLOCATOR_H_
#define ENGINE_FIXED_SIZE_ALLOCATOR_H_

// library includes
#include <stdint.h>

// engine includes
#include "AllocatorUtil.h"

// forward declaration
namespace engine {
namespace data {
	class BitArray;
}
}

namespace engine {
namespace memory {

#define MAX_FIXED_SIZE_ALLOCATORS 15

// forward declarations
class BlockAllocator;

/*
	FixedSizeAllocator
	- A simple fixed size allocator that users can use to request for memory of a fixed size
	- Users need to provide the size of each fixed size block, the total number of blocks
	  and a block allocator to allocate necessary memory from (usually the default block allocator)
	- No more than 15 (MAX_FIXED_SIZE_ALLOCATORS) instances of this class must be created & no two instances can have the same block size
	- In order to be within overridden versions of new, delete, malloc & free, an instance must be "registered" using
	  the static AddFixedSizeAllocator function
*/
class FixedSizeAllocator
{
private:
	// disable copy constructor & assignment operator
	FixedSizeAllocator(const FixedSizeAllocator& i_copy) = delete;
	FixedSizeAllocator& operator=(const FixedSizeAllocator& i_fsa) = delete;

	FixedSizeAllocator(void* i_memory, const size_t i_total_block_size, const size_t i_fixed_block_size, const size_t i_num_blocks, BlockAllocator* i_allocator);
	~FixedSizeAllocator();

	inline uint8_t* GetPointerForBlock(const size_t i_bit_index) const;

#ifdef BUILD_DEBUG
	bool CheckMemoryOverwrite(const size_t i_bit_index) const;
	inline void ClearBlock(const size_t i_bit_index, const unsigned char i_fill);
#endif

public:
	static FixedSizeAllocator* Create(const size_t i_block_size, const size_t i_num_blocks, BlockAllocator* i_allocator);
	static void Destroy(FixedSizeAllocator* i_allocator);

	static bool IsFixedSizeAllocatorAvailable(FixedSizeAllocator* i_allocator);
	static bool AddFixedSizeAllocator(FixedSizeAllocator* i_allocator);
	static bool RemoveFixedSizeAllocator(FixedSizeAllocator* i_allocator);
	static inline FixedSizeAllocator** const GetAvailableFixedSizeAllocators();

	// allocate a block of fixed size
	void* Alloc();
	// allocate a block of given size
	void* Alloc(const size_t i_size);
	// deallocate a block of memory
	bool Free(void* i_pointer);

	// Query whether a given pointer is within this allocator's range
	inline bool Contains(const void* i_pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(const void* i_pointer) const;

	inline const size_t GetNumAvailableBlocks() const;
	inline const size_t GetNumOustandingBlocks() const;

	inline const size_t GetBlockSize() const;
	inline const size_t GetNumBlocks() const;

#ifdef BUILD_DEBUG
	inline unsigned int GetID() const;
    void DumpStatistics() const;
#endif

private:
	uint8_t*										block_;													// pointer to the actual block of memory
	size_t											total_block_size_;										// total size of the entire block
	size_t											fixed_block_size_;										// size of each fixed block
	size_t											num_blocks_;											// total number of fixed blocks
	BlockAllocator*									block_allocator_;										// the block allocator used for the initial allocation
	engine::data::BitArray*							block_state_;											// a bit array to maintain the state (available = 0, allocated = 1) of each block of memory

#ifdef BUILD_DEBUG
	uint8_t											id_;													// an id to keep track of this allocator in debug mode
	static uint8_t									counter_;												// a counter that will be used while setting ids for allocators
    AllocatorStatistics                             stats_;                                                 // a struct that keeps track of various statistics to help diagnose memory usage
#endif

	struct FSASort
	{
		bool operator()(FixedSizeAllocator* i_a, FixedSizeAllocator* i_b)
		{
			if (!i_a)					return false;
			else if (!i_b)				return true;
			return (i_a->GetBlockSize() < i_b->GetBlockSize());
		}
	};

	static FixedSizeAllocator*						available_allocators_[MAX_FIXED_SIZE_ALLOCATORS];		// an array of pointers to all available fixed size allocators
	static FSASort									FSASorter;												// a custom function object to sort available fixed size allocators

}; // class FixedSizeAllocator

} // namespace memory
} // namespace engine

#include "FixedSizeAllocator-inl.h"

#endif // ENGINE_FIXED_SIZE_ALLOCATOR_H_