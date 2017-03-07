#include "BlockAllocator.h"

// library includes
#include <string.h>         // for memset

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
    inline void BlockAllocator::ClearBlock(BD* i_bd, const unsigned char i_fill)
    {
        ASSERT(i_bd != nullptr);
        memset(i_bd->block_pointer, i_fill, i_bd->block_size);
    }

    inline unsigned int BlockAllocator::GetID() const
    {
        return id_;
    }
#endif

    // Query whether a given pointer is within this allocator's range
    inline bool BlockAllocator::Contains(const void* i_pointer) const
    {
        ASSERT(i_pointer != nullptr);
        return (static_cast<const uint8_t*>(i_pointer) >= block_ && static_cast<const uint8_t*>(i_pointer) <= (block_ + total_block_size_));
    }

    inline const size_t BlockAllocator::GetTotalFreeMemorySize() const
    {
        size_t total_size = 0;
        // loop the free list
        BD* bd = free_list_head_;
        while (bd != nullptr)
        {
            total_size += bd->block_size;
            bd = bd->next;
        }

        return total_size;
    }

    inline const size_t BlockAllocator::GetNumOustandingBlocks() const
    {
        size_t num_outstanding_blocks = 0;
        for (BD* bd = user_list_head_; bd != nullptr; bd = bd->next)
        {
            ++num_outstanding_blocks;
        }
        return num_outstanding_blocks;
    }

#ifdef BUILD_DEBUG
    inline const AllocatorStatistics& BlockAllocator::GetStatistics() const
    {
        return stats_;
    }
#endif

} // namespace memory
} // namespace engine