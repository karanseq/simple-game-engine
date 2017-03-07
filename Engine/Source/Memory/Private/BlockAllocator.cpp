#include "Memory\BlockAllocator.h"

// library includes
#include <limits>           // for numeric_limits
#include <new>              // for placement new
#include <stdlib.h>         // for _aligned_malloc & _aligned_free

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\AllocationCounter.h"
#include "Memory\AllocatorUtil.h"

namespace engine {
namespace memory {

// initialize static members
const size_t                BlockAllocator::DEFAULT_ALLOCATOR_SIZE = 10 * 1024 * 1024;
size_t                      BlockAllocator::size_of_BD_ = sizeof(BD);
BlockAllocator*             BlockAllocator::available_allocators_[MAX_BLOCK_ALLOCATORS] = { nullptr };

#ifdef BUILD_DEBUG
uint8_t                     BlockAllocator::counter_ = 0;
#endif

BlockAllocator::BlockAllocator(void* i_memory, size_t i_block_size) : block_(static_cast<uint8_t*>(i_memory)),
    user_list_head_(nullptr),
    free_list_head_(nullptr),
    total_block_size_(i_block_size)
{
    // validate input
    ASSERT(block_);
    ASSERT(total_block_size_ > 0);

#ifdef BUILD_DEBUG
    descriptor_counter_ = 0;
    id_ = BlockAllocator::counter_++;
    memset(block_, CLEAN_FILL, total_block_size_);
    VERBOSE("BlockAllocator-%d created with size:%zu", id_, total_block_size_);

    // initialize diagnostic information
    stats_.available_memory_size = total_block_size_;
#endif

    InitFirstBlockDescriptor();
}

BlockAllocator* BlockAllocator::Create(void* i_memory, size_t i_block_size)
{
    // validate input
    ASSERT(i_memory);
    ASSERT(i_block_size > (sizeof(BlockAllocator)));

    // add the allocator to the start of the aligned memory block
    uint8_t* block_allocator_memory = static_cast<uint8_t*>(i_memory);
    // move up the address of the usable block
    block_allocator_memory += sizeof(BlockAllocator);
    // reduce the size of the usable block
    i_block_size -= sizeof(BlockAllocator);

    // create the allocator
    BlockAllocator* block_allocator = new (i_memory) BlockAllocator(block_allocator_memory, i_block_size);
    ASSERT(block_allocator);

    return block_allocator;
}

void BlockAllocator::Destroy(BlockAllocator* i_allocator)
{
    // validate input
    ASSERT(i_allocator);

#ifdef BUILD_DEBUG
    // TODO: Print *more* diagnostics
    if (i_allocator->user_list_head_ != nullptr)
    {
        LOG_ERROR("WARNING! Found %zu unfreed allocations in BlockAllocator-%d", i_allocator->GetNumOustandingBlocks(), i_allocator->id_);
    }

    // dump statistics
    i_allocator->DumpStatistics();

    LOG("BlockAllocator-%d destroyed", i_allocator->id_);
#else
    LOG("BlockAllocator destroyed");
#endif
}

BlockAllocator* BlockAllocator::GetDefaultAllocator()
{
    if (available_allocators_[0] == nullptr)
    {
        CreateDefaultAllocator();
    }
    return available_allocators_[0];
}

void BlockAllocator::CreateDefaultAllocator()
{
    size_t default_block_size = DEFAULT_ALLOCATOR_SIZE;

    // allocate aligned memory for the default allocator
    void* default_allocator_memory = _aligned_malloc(default_block_size, 4);
    ASSERT(default_allocator_memory);

    // add the allocator to the start of the aligned memory block
    uint8_t* block_allocator_memory = static_cast<uint8_t*>(default_allocator_memory);
    // move up the address of the usable block
    block_allocator_memory += sizeof(BlockAllocator);
    // reduce the size of the usable block
    default_block_size -= sizeof(BlockAllocator);

    // create the default allocator
    available_allocators_[0] = new (default_allocator_memory) BlockAllocator(block_allocator_memory, default_block_size);
    if (!available_allocators_[0])
    {
        // spit out an error
        LOG_ERROR("Could not create the default allocator!");

        // free the aligned memory
        _aligned_free(default_allocator_memory);
        default_allocator_memory = nullptr;
        return;
    }
}

void BlockAllocator::DestroyDefaultAllocator()
{
    Destroy(available_allocators_[0]);

    _aligned_free(available_allocators_[0]);
    available_allocators_[0] = nullptr;
}

bool BlockAllocator::IsBlockAllocatorAvailable(BlockAllocator* i_allocator)
{
    // validate input
    ASSERT(i_allocator);

    // search for the allocator
    for (uint8_t i = 0; i < MAX_BLOCK_ALLOCATORS; ++i)
    {
        if (available_allocators_[i] == i_allocator)
        {
            return true;
        }
    }
    return false;
}

bool BlockAllocator::AddBlockAllocator(BlockAllocator* i_allocator)
{
    // validate input
    ASSERT(i_allocator);

    // find a suitable position in the list of registered allocators
    // i starts at 1 since the first position in the array is reserved for the default allocator
    for (uint8_t i = 1; i < MAX_BLOCK_ALLOCATORS; ++i)
    {
        if (!available_allocators_[i])
        {
            available_allocators_[i] = i_allocator;
            return true;
        }
    }
    return false;
}

bool BlockAllocator::RemoveBlockAllocator(BlockAllocator* i_allocator)
{
    // validate input
    ASSERT(i_allocator);

    // search for the allocator in the list of registered allocators
    // i starts at 1 since the first position in the array is reserved for the default allocator
    for (uint8_t i = 1; i < MAX_BLOCK_ALLOCATORS; ++i)
    {
        if (available_allocators_[i] == i_allocator)
        {
            available_allocators_[i] = nullptr;
            return true;
        }
    }
    return false;
}

void BlockAllocator::InitFirstBlockDescriptor()
{
    // initialize the first descriptor
    BD* first_bd = reinterpret_cast<BD*>(block_);
    first_bd->block_pointer = block_ + size_of_BD_;
    first_bd->block_size = total_block_size_ - size_of_BD_;
#ifdef BUILD_DEBUG
    first_bd->id = descriptor_counter_++;
#endif

    // add the descriptor to the free list
    AddToList(&free_list_head_, &first_bd, false);
}

void BlockAllocator::AddToList(BD** i_head, BD** i_bd, bool i_enable_sort)
{
    // validate input
    ASSERT(i_head != nullptr);
    ASSERT(i_bd != nullptr);

    // check if the list is empty
    if (*i_head == nullptr)
    {
        // add the new descriptor to the front
        (*i_bd)->next = nullptr;
        (*i_bd)->previous = nullptr;
        *i_head = *i_bd;
        return;
    }

    // add the new descriptor to the front if:
    // sorting is disabled OR the new descriptor's block pointer is before the head's
    if (!i_enable_sort || (*i_bd)->block_pointer < (*i_head)->block_pointer)
    {
        (*i_head)->previous = *i_bd;
        (*i_bd)->next = *i_head;
        (*i_bd)->previous = nullptr;
        *i_head = *i_bd;
    }
    else if (i_enable_sort)
    {
        // add this descriptor based on ascending order of address
        BD* curr = *i_head;
        BD* prev = nullptr;
        while (curr != nullptr)
        {
            if ((*i_bd)->block_pointer < curr->block_pointer)
            {
                if (prev != nullptr)
                {
                    prev->next = *i_bd;
                }
                (*i_bd)->next = curr;
                (*i_bd)->previous = prev;
                curr->previous = *i_bd;
                return;
            }
            prev = curr;
            curr = curr->next;
        }

        // this means the descriptor's block is larger than all elements in the list
        prev->next = *i_bd;
        (*i_bd)->next = nullptr;
        (*i_bd)->previous = prev;
    }
}

void BlockAllocator::RemoveFromList(BD** i_head, BD** i_bd)
{
    // validate input
    ASSERT(i_head != nullptr);
    ASSERT(i_bd != nullptr);

    if ((*i_bd)->previous == nullptr)
    {
        // this means we're removing the head
        *i_head = (*i_bd)->next;
    }
    else
    {
        // update previous block's next pointer
        ((*i_bd)->previous)->next = (*i_bd)->next;
    }

    // update next block's previous pointer
    if ((*i_bd)->next)
    {
        ((*i_bd)->next)->previous = (*i_bd)->previous;
    }

    // nullptrify current block's pointers
    (*i_bd)->previous = nullptr;
    (*i_bd)->next = nullptr;
}

#ifdef BUILD_DEBUG
bool BlockAllocator::CheckMemoryOverwrite(BD* i_bd) const
{
    // validate input
    ASSERT(i_bd != nullptr);
    uint8_t lower_byte_counter = 0, upper_byte_counter = 0;
    for (uint8_t i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
    {
        // check lower guardband
        lower_byte_counter += (*(i_bd->block_pointer + i) == GUARDBAND_FILL) ? 1 : 0;

        // check upper guardband
        upper_byte_counter += (*(i_bd->block_pointer + DEFAULT_GUARDBAND_SIZE + i_bd->user_size + i) == GUARDBAND_FILL) ? 1 : 0;
    }

    bool found_overwrite = !(lower_byte_counter >= DEFAULT_GUARDBAND_SIZE && upper_byte_counter >= DEFAULT_GUARDBAND_SIZE);
    if (found_overwrite)
    {
        LOG_ERROR("BlockAllocator-%d detected overwritten memory!");
    }

    return found_overwrite;
}
#endif

void* BlockAllocator::Alloc(const size_t i_size, const size_t i_alignment)
{
    std::lock_guard<std::mutex> lock(allocator_mutex_);

    // size should be greater than zero!
    ASSERT(i_size > 0);
    // alignment should be power of two!
    ASSERT((i_alignment & (i_alignment - 1)) == 0);

#ifdef BUILD_DEBUG
    const size_t    guardband_size = DEFAULT_GUARDBAND_SIZE;
#else
    const size_t    guardband_size = 0;
#endif

    // declare a block descriptor to service this request
    BD*             new_bd = nullptr;

    // loop the free list for a descriptor to a block that is big enough
    bool            did_defrag = false;
    BD*             free_bd = free_list_head_;
    while (free_bd != nullptr)
    {
        // check if this block is big enough
        if (i_size <= free_bd->block_size)
        {
            // calculate the new address of the new block
            uint8_t*                new_block_pointer = free_bd->block_pointer + free_bd->block_size - size_of_BD_ - guardband_size * 2 - i_size;
            const size_t            alignment_offset = reinterpret_cast<uintptr_t>(new_block_pointer + size_of_BD_ + guardband_size) & (i_alignment - 1);

            // check if this block needs to be fragmented
            if ((size_of_BD_ + guardband_size * 2 + i_size + alignment_offset + MAX_EXTRA_MEMORY) <= free_bd->block_size)
            {
                // block is much bigger than we need so we'll fragment it
                new_block_pointer = free_bd->block_pointer + free_bd->block_size - size_of_BD_ - guardband_size * 2 - i_size - alignment_offset;

                // initialize the new block's descriptor
                new_bd = reinterpret_cast<BD*>(new_block_pointer);
                new_bd->block_pointer = new_block_pointer + size_of_BD_;
                new_bd->block_size = i_size + alignment_offset + guardband_size * 2;
#ifdef BUILD_DEBUG
                new_bd->id = descriptor_counter_++;
                descriptor_counter_ = (descriptor_counter_ >= std::numeric_limits<uint32_t>::max() ? 0 : descriptor_counter_);
#endif

                // splice the free block
                free_bd->block_size -= (size_of_BD_ + new_bd->block_size);

                // done with the search so break
                break;
            }
            // check to see if this block is big enough to be used as is
            else if ((i_size + guardband_size * 2) <= free_bd->block_size)
            {
                // if we are to use the entire block as is, check to see if it is properly byte aligned 
                if ((reinterpret_cast<uintptr_t>(free_bd->block_pointer + guardband_size) & (i_alignment - 1)) == 0)
                {
                    // block is slightly bigger so we'll use it whole
                    new_bd = free_bd;

                    // remove the descriptor from the free list
                    RemoveFromList(&free_list_head_, &free_bd);

                    // done with the search so break
                    break;
                }
            }
        }

        // move to the next descriptor in the free list
        free_bd = free_bd->next;

        // have we reached the end of the free list?
        if (free_bd == nullptr)
        {
            // this means we still haven't found a free block that's big enough         
            if (!did_defrag)
            {
                // defragment but only ONCE
                did_defrag = true;
                Defragment();

                // start again at the front of the free list
                free_bd = free_list_head_;
            }
            else
            {
#ifdef BUILD_DEBUG
                LOG_ERROR("BlockAllocator-%d ran out of memory!", id_);
#else
                LOG_ERROR("A BlockAllocator ran out of memory!");
#endif
                return nullptr;
            }
        }

    } // end of while loop to search for free blocks

    // this means we couldn't find a block even after defragmenting
    if (new_bd == nullptr)
    {
#ifdef BUILD_DEBUG
        LOG_ERROR("BlockAllocator-%d ran out of memory!", id_);
#else
        LOG_ERROR("A BlockAllocator ran out of memory!");
#endif
        return nullptr;
    }

#ifdef BUILD_DEBUG
    // clear the block
    ClearBlock(new_bd, CLEAN_FILL);

    // add guardbands
    for (uint8_t i = 0; i < guardband_size; ++i)
    {
        *(new_bd->block_pointer + i) = GUARDBAND_FILL;
        *(new_bd->block_pointer + guardband_size + i_size + i) = GUARDBAND_FILL;
    }

    // save the size requested by the user for future use
    new_bd->user_size = i_size;
#endif

    // add the descriptor to the user list
    AddToList(&user_list_head_, &new_bd, false);

#ifdef BUILD_DEBUG
    // update diagnostic information
    ++stats_.num_allocated;
    ++stats_.num_outstanding;
    stats_.max_num_outstanding = stats_.max_num_outstanding < stats_.num_outstanding ? stats_.num_outstanding : stats_.max_num_outstanding;
    stats_.allocated_memory_size += (size_of_BD_ + new_bd->block_size);
    stats_.available_memory_size -= (size_of_BD_ + new_bd->block_size);
    stats_.max_allocated_memory_size = stats_.max_allocated_memory_size < stats_.allocated_memory_size ? stats_.allocated_memory_size : stats_.max_allocated_memory_size;
    COUNT_ALLOC(i_size);
#endif

    return (new_bd->block_pointer + guardband_size);
}

// Deallocate a block of memory
bool BlockAllocator::Free(void* i_pointer)
{
    std::lock_guard<std::mutex> lock(allocator_mutex_);

    // validate input
    ASSERT(i_pointer != nullptr);

    // return if this allocator does not contain this pointer
    if (!Contains(i_pointer))
    {
        return false;
    }

    // calculate the address of the descriptor
#ifdef BUILD_DEBUG
    BD* bd = reinterpret_cast<BD*>(static_cast<uint8_t*>(i_pointer) - DEFAULT_GUARDBAND_SIZE - size_of_BD_);
#else
    BD* bd = reinterpret_cast<BD*>(static_cast<uint8_t*>(i_pointer) - size_of_BD_);
#endif

#ifdef BUILD_DEBUG
    // check for overwrites
    ASSERT(!CheckMemoryOverwrite(bd));
#endif

    // remove the descriptor from the user list
    RemoveFromList(&user_list_head_, &bd);

#ifdef BUILD_DEBUG
    // clear the block
    ClearBlock(bd, DEAD_FILL);

    // reset the user size
    bd->user_size = 0;
#endif

    // add the descriptor to the free list
    AddToList(&free_list_head_, &bd, true);

#ifdef BUILD_DEBUG
    // update diagnostic information
    ++stats_.num_freed;
    --stats_.num_outstanding;
    stats_.allocated_memory_size -= (bd->block_size);
    stats_.available_memory_size += (bd->block_size);
#endif

    return true;
}

// Run defragmentation
void BlockAllocator::Defragment()
{
#ifdef BUILD_DEBUG
    VERBOSE("Defragmenting...");
    size_t              num_blocks_combined = 0;
    size_t              bytes_combined = 0;
#endif

    BD* curr = free_list_head_;
    while (curr != nullptr && curr->next != nullptr)
    {
        // check if next block is contiguous to current block
        if (curr->block_pointer + curr->block_size == curr->next->block_pointer - size_of_BD_)
        {
            BD* next_bd = curr->next;

#ifdef BUILD_DEBUG
            VERBOSE("Merging %zu bytes from block-%d & block-%d", (next_bd->block_size + size_of_BD_), curr->id, next_bd->id);
#endif
            // update current descriptor's block size
            curr->block_size += (next_bd->block_size + size_of_BD_);

#ifdef BUILD_DEBUG
            bytes_combined += (next_bd->block_size + size_of_BD_);
#endif
            // update current descriptor's next & previous pointers
            curr->next = next_bd->next;
            if (next_bd->next)
            {
                (next_bd->next)->previous = curr;
            }

            // clear next descriptor
            next_bd->next = nullptr;
            next_bd->previous = nullptr;
            next_bd->block_pointer = nullptr;
            next_bd->block_size = 0;

#ifdef BUILD_DEBUG
            ++num_blocks_combined;
#endif
            continue;
        }
        curr = curr->next;
    }

#ifdef BUILD_DEBUG
    if (bytes_combined > 0)
    {
        VERBOSE("Defragment finished...merged %zu bytes from %zu blocks!", bytes_combined, num_blocks_combined);
    }
    else
    {
        VERBOSE("Defragment finished...could not find any contiguous blocks!");
    }
#endif
}

// Query whether a given pointer is a user allocation
bool BlockAllocator::IsAllocated(const void* i_pointer) const
{
    // validate input
    ASSERT(i_pointer);

    BD* bd = user_list_head_;
    while (bd != nullptr)
    {
        // check if the pointer passed in exists within each descriptor
        if ((static_cast<const uint8_t*>(i_pointer) >= bd->block_pointer && static_cast<const uint8_t*>(i_pointer) < (bd->block_pointer + bd->block_size)))
        {
            return true;
        }
        bd = bd->next;
    }
    return false;
}

const size_t BlockAllocator::GetLargestFreeBlockSize(const size_t i_alignment) const
{
    // alignment should be power of two!
    ASSERT((i_alignment & (i_alignment - 1)) == 0);

    size_t largest_size = 0;
    // loop the free list
    BD* bd = free_list_head_;
    while (bd != nullptr)
    {
        // check actual block size not user block size
        if (bd->block_size > largest_size)
        {
            largest_size = bd->block_size;
        }
        bd = bd->next;
    }

#ifdef BUILD_DEBUG
    return largest_size - i_alignment - DEFAULT_GUARDBAND_SIZE * 2;
#else
    return largest_size - i_alignment;
#endif
}

#ifdef BUILD_DEBUG
void BlockAllocator::DumpStatistics() const
{
    LOG("---------- %s ----------", __FUNCTION__);
    LOG("Dumping usage statistics for BlockAllocator-%d:", id_);
    LOG("Total allocations:%zu", stats_.num_allocated);
    LOG("Total frees:%zu", stats_.num_freed);
    LOG("Highwater mark:%zu allocations and %zu bytes", stats_.max_num_outstanding, stats_.max_allocated_memory_size);
    LOG("---------- END ----------");
}

void BlockAllocator::PrintAllDescriptors() const
{
    PrintFreeDescriptors();
    PrintUsedDescriptors(); 
}

void BlockAllocator::PrintFreeDescriptors() const
{
    VERBOSE("---------- %s ----------", __FUNCTION__);
    if (free_list_head_ != nullptr)
    {
        size_t count = 0;
        VERBOSE("FREE:");
        for (BD* bd = free_list_head_; bd != nullptr; bd = bd->next)
        {
            VERBOSE("BD.id:%d size:%zu", bd->id, bd->block_size);
            ++count;
        }
        VERBOSE("FREE LIST SIZE:%d", count);
    }
    else
    {
        VERBOSE("FREE LIST EMPTY!");
    }
    VERBOSE("---------- END ----------");
}

void BlockAllocator::PrintUsedDescriptors() const
{
    VERBOSE("---------- %s ----------", __FUNCTION__);
    if (user_list_head_ != nullptr)
    {
        size_t count = 0;
        VERBOSE("USER:");
        for (BD* bd = user_list_head_; bd != nullptr; bd = bd->next)
        {
            VERBOSE("BD.id:%d size:%zu", bd->id, bd->block_size);
            ++count;
        }
        VERBOSE("USER LIST SIZE:%d", count);
    }
    else
    {
        VERBOSE("USER LIST EMPTY!");
    }
    VERBOSE("---------- END ----------");
}
#endif

} // namespace memory
} // namespace engine