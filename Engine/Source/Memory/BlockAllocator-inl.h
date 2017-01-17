#include "BlockAllocator.h"

// library includes
#include <string.h>			// for memset

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace memory {

	inline BlockAllocator** const BlockAllocator::GetAvailableBlockAllocators()
	{
		return available_allocators_;
	}

	inline size_t BlockAllocator::GetSizeOfBD()
	{
		return size_of_BD_;
	}

#ifdef BUILD_DEBUG
	inline void BlockAllocator::ClearBlock(BD* bd, const unsigned char fill)
	{
		ASSERT(bd != nullptr);
		memset(bd->block_pointer_, fill, bd->block_size_);
	}

	inline unsigned int BlockAllocator::GetID() const
	{
		return id_;
	}
#endif

	// Query whether a given pointer is within this allocator's range
	inline bool BlockAllocator::Contains(const void* pointer) const
	{
		ASSERT(pointer != nullptr);
		return (static_cast<const uint8_t*>(pointer) >= block_ && static_cast<const uint8_t*>(pointer) <= (block_ + total_block_size_));
	}

	inline const size_t BlockAllocator::GetTotalFreeMemorySize() const
	{
		size_t total_size = 0;
		// loop the free list
		BD* bd = free_list_head_;
		while (bd != nullptr)
		{
			total_size += bd->block_size_;
			bd = bd->next_;
		}

		return total_size;
	}

	inline const size_t BlockAllocator::GetNumOustandingBlocks() const
	{
		size_t num_outstanding_blocks = 0;
		for (BD* bd = user_list_head_; bd != nullptr; bd = bd->next_)
		{
			++num_outstanding_blocks;
		}
		return num_outstanding_blocks;
	}

} // namespace memory
} // namespace engine