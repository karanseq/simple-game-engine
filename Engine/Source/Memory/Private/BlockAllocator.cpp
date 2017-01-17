#include "Memory\BlockAllocator.h"

// library includes
#include <limits>			// for numeric_limits
#include <new>				// for placement new
#include <stdlib.h>			// for _aligned_malloc & _aligned_free

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorUtil.h"

namespace engine {
namespace memory {

// initialize static members
size_t				BlockAllocator::size_of_BD_ = sizeof(BD);
BlockAllocator*		BlockAllocator::available_allocators_[MAX_BLOCK_ALLOCATORS] = { nullptr };

#ifdef BUILD_DEBUG
uint8_t				BlockAllocator::counter_ = 0;
#endif

BlockAllocator::BlockAllocator(void* memory, size_t block_size) : block_(static_cast<uint8_t*>(memory)),
	user_list_head_(nullptr),
	free_list_head_(nullptr),
	total_block_size_(block_size)
{
	// validate input
	ASSERT(block_);
	ASSERT(total_block_size_ > 0);

#ifdef BUILD_DEBUG
	descriptor_counter_ = 0;
	id_ = BlockAllocator::counter_++;
	memset(block_, CLEAN_FILL, total_block_size_);
	VERBOSE("BlockAllocator-%d created with size:%zu", id_, total_block_size_);
#endif

	InitFirstBlockDescriptor();
}

BlockAllocator* BlockAllocator::Create(void* memory, size_t block_size)
{
	// validate input
	ASSERT(memory);
	ASSERT(block_size > 0);
	ASSERT(block_size > (sizeof(BlockAllocator)));

	// add the allocator to the start of the aligned memory block
	uint8_t* block_allocator_memory = static_cast<uint8_t*>(memory);
	// move up the address of the usable block
	block_allocator_memory += sizeof(BlockAllocator);
	// reduce the size of the usable block
	block_size -= sizeof(BlockAllocator);

	// create the allocator
	BlockAllocator* block_allocator = new (memory) BlockAllocator(block_allocator_memory, block_size);
	ASSERT(block_allocator);

	return block_allocator;
}

void BlockAllocator::Destroy(BlockAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

#ifdef BUILD_DEBUG
	// TODO: Print *more* diagnostics
	if (allocator->user_list_head_ != nullptr)
	{
		LOG_ERROR("WARNING! Found %zu unfreed allocations in BlockAllocator-%d", allocator->GetNumOustandingBlocks(), allocator->id_);
	}

	VERBOSE("BlockAllocator-%d destroyed", allocator->id_);
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
	size_t default_block_size = 1024 * 1024;

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

bool BlockAllocator::IsBlockAllocatorAvailable(BlockAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	// search for the allocator
	for (uint8_t i = 0; i < MAX_BLOCK_ALLOCATORS; ++i)
	{
		if (available_allocators_[i] == allocator)
		{
			return true;
		}
	}
	return false;
}

bool BlockAllocator::AddBlockAllocator(BlockAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	// find a suitable position in the list of registered allocators
	// i starts at 1 since the first position in the array is reserved for the default allocator
	for (uint8_t i = 1; i < MAX_BLOCK_ALLOCATORS; ++i)
	{
		if (!available_allocators_[i])
		{
			available_allocators_[i] = allocator;
			return true;
		}
	}
	return false;
}

bool BlockAllocator::RemoveBlockAllocator(BlockAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	// search for the allocator in the list of registered allocators
	// i starts at 1 since the first position in the array is reserved for the default allocator
	for (uint8_t i = 1; i < MAX_BLOCK_ALLOCATORS; ++i)
	{
		if (available_allocators_[i] == allocator)
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
	first_bd->block_pointer_ = block_ + size_of_BD_;
	first_bd->block_size_ = total_block_size_ - size_of_BD_;
#ifdef BUILD_DEBUG
	first_bd->id_ = descriptor_counter_++;
#endif

	// add the descriptor to the free list
	AddToList(&free_list_head_, &first_bd, false);
}

void BlockAllocator::AddToList(BD** head, BD** bd, bool enable_sort)
{
	// validate input
	ASSERT(head != nullptr);
	ASSERT(bd != nullptr);

	// check if the list is empty
	if (*head == nullptr)
	{
		// add the new descriptor to the front
		(*bd)->next_ = nullptr;
		(*bd)->previous_ = nullptr;
		*head = *bd;
		return;
	}

	// add the new descriptor to the front if:
	// sorting is disabled OR the new descriptor's block pointer is before the head's
	if (!enable_sort || (*bd)->block_pointer_ < (*head)->block_pointer_)
	{
		(*head)->previous_ = *bd;
		(*bd)->next_ = *head;
		(*bd)->previous_ = nullptr;
		*head = *bd;
	}
	else if (enable_sort)
	{
		// add this descriptor based on ascending order of address
		BD* curr = *head;
		BD* prev = nullptr;
		while (curr != nullptr)
		{
			if ((*bd)->block_pointer_ < curr->block_pointer_)
			{
				if (prev != nullptr)
				{
					prev->next_ = *bd;
				}
				(*bd)->next_ = curr;
				(*bd)->previous_ = prev;
				curr->previous_ = *bd;
				return;
			}
			prev = curr;
			curr = curr->next_;
		}

		// this means the descriptor's block is larger than all elements in the list
		prev->next_ = *bd;
		(*bd)->next_ = nullptr;
		(*bd)->previous_ = prev;
	}
}

void BlockAllocator::RemoveFromList(BD** head, BD** bd)
{
	// validate input
	ASSERT(head != nullptr);
	ASSERT(bd != nullptr);

	if ((*bd)->previous_ == nullptr)
	{
		// this means we're removing the head
		*head = (*bd)->next_;
	}
	else
	{
		// update previous block's next pointer
		((*bd)->previous_)->next_ = (*bd)->next_;
	}

	// update next block's previous pointer
	if ((*bd)->next_)
	{
		((*bd)->next_)->previous_ = (*bd)->previous_;
	}

	// nullptrify current block's pointers
	(*bd)->previous_ = nullptr;
	(*bd)->next_ = nullptr;
}

#ifdef BUILD_DEBUG
bool BlockAllocator::CheckMemoryOverwrite(BD* bd) const
{
	// validate input
	ASSERT(bd != nullptr);
	uint8_t lower_byte_counter = 0, upper_byte_counter = 0;
	for (uint8_t i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		// check lower guardband
		lower_byte_counter += (*(bd->block_pointer_ + i) == GUARDBAND_FILL) ? 1 : 0;

		// check upper guardband
		upper_byte_counter += (*(bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE + bd->user_size_ + i) == GUARDBAND_FILL) ? 1 : 0;
	}

	bool found_overwrite = !(lower_byte_counter >= DEFAULT_GUARDBAND_SIZE && upper_byte_counter >= DEFAULT_GUARDBAND_SIZE);
	if (found_overwrite)
	{
		LOG_ERROR("BlockAllocator-%d detected overwritten memory!");
	}

	return found_overwrite;
}
#endif

void* BlockAllocator::Alloc(const size_t size, const size_t alignment)
{
	// size should be greater than zero!
	ASSERT(size > 0);
	// alignment should be power of two!
	ASSERT((alignment & (alignment - 1)) == 0);

#ifdef BUILD_DEBUG
	const size_t	guardband_size = DEFAULT_GUARDBAND_SIZE;
#else
	const size_t	guardband_size = 0;
#endif

	// declare a block descriptor to service this request
	BD*				new_bd = nullptr;

	// loop the free list for a descriptor to a block that is big enough
	bool			did_defrag = false;
	BD*				free_bd = free_list_head_;
	while (free_bd != nullptr)
	{
		// check if this block is big enough
		if (size <= free_bd->block_size_)
		{
			// calculate the new address of the new block
			uint8_t*				new_block_pointer = free_bd->block_pointer_ + free_bd->block_size_ - size_of_BD_ - guardband_size * 2 - size;
			const size_t			alignment_offset = reinterpret_cast<uintptr_t>(new_block_pointer + size_of_BD_ + guardband_size) & (alignment - 1);

			// check if this block needs to be fragmented
			if ((size_of_BD_ + guardband_size * 2 + size + alignment_offset + MAX_EXTRA_MEMORY) <= free_bd->block_size_)
			{
				// block is much bigger than we need so we'll fragment it
				new_block_pointer = free_bd->block_pointer_ + free_bd->block_size_ - size_of_BD_ - guardband_size * 2 - size - alignment_offset;

				// initialize the new block's descriptor
				new_bd = reinterpret_cast<BD*>(new_block_pointer);
				new_bd->block_pointer_ = new_block_pointer + size_of_BD_;
				new_bd->block_size_ = size + alignment_offset + guardband_size * 2;
#ifdef BUILD_DEBUG
				new_bd->id_ = descriptor_counter_++;
				descriptor_counter_ = (descriptor_counter_ >= std::numeric_limits<uint32_t>::max() ? 0 : descriptor_counter_);
#endif

				// splice the free block
				free_bd->block_size_ -= (size_of_BD_ + new_bd->block_size_);

				// done with the search so break
				break;
			}
			// check to see if this block is big enough to be used as is
			else if ((size + guardband_size * 2) <= free_bd->block_size_)
			{
				// if we are to use the entire block as is, check to see if it is properly byte aligned 
				if ((reinterpret_cast<uintptr_t>(free_bd->block_pointer_ + guardband_size) & (alignment - 1)) == 0)
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
		free_bd = free_bd->next_;

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
		*(new_bd->block_pointer_ + i) = GUARDBAND_FILL;
		*(new_bd->block_pointer_ + guardband_size + size + i) = GUARDBAND_FILL;
	}

	// save the size requested by the user for future use
	new_bd->user_size_ = size;
#endif

	// add the descriptor to the user list
	AddToList(&user_list_head_, &new_bd, false);

	return (new_bd->block_pointer_ + guardband_size);
}

// Deallocate a block of memory
bool BlockAllocator::Free(void* pointer)
{
	// validate input
	ASSERT(pointer != nullptr);

	// return if this allocator does not contain this pointer
	if (!Contains(pointer))
	{
		return false;
	}

	// calculate the address of the descriptor
#ifdef BUILD_DEBUG
	BD* bd = reinterpret_cast<BD*>(static_cast<uint8_t*>(pointer) - DEFAULT_GUARDBAND_SIZE - size_of_BD_);
#else
	BD* bd = reinterpret_cast<BD*>(static_cast<uint8_t*>(pointer) - size_of_BD_);
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
	bd->user_size_ = 0;
#endif

	// add the descriptor to the free list
	AddToList(&free_list_head_, &bd, true);

	return true;
}

// Run defragmentation
void BlockAllocator::Defragment()
{
#ifdef BUILD_DEBUG
	VERBOSE("Defragmenting...");
	size_t				num_blocks_combined = 0;
	size_t				bytes_combined = 0;
#endif

	BD* curr = free_list_head_;
	while (curr != nullptr && curr->next_ != nullptr)
	{
		// check if next block is contiguous to current block
		if (curr->block_pointer_ + curr->block_size_ == curr->next_->block_pointer_ - size_of_BD_)
		{
			BD* next_bd = curr->next_;

#ifdef BUILD_DEBUG
			VERBOSE("Merging %zu bytes from block-%d & block-%d", (next_bd->block_size_ + size_of_BD_), curr->id_, next_bd->id_);
#endif
			// update current descriptor's block size
			curr->block_size_ += (next_bd->block_size_ + size_of_BD_);

#ifdef BUILD_DEBUG
			bytes_combined += (next_bd->block_size_ + size_of_BD_);
#endif
			// update current descriptor's next & previous pointers
			curr->next_ = next_bd->next_;
			if (next_bd->next_)
			{
				(next_bd->next_)->previous_ = curr;
			}

			// clear next descriptor
			next_bd->next_ = nullptr;
			next_bd->previous_ = nullptr;
			next_bd->block_pointer_ = nullptr;
			next_bd->block_size_ = 0;

#ifdef BUILD_DEBUG
			++num_blocks_combined;
#endif
			continue;
		}
		curr = curr->next_;
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
bool BlockAllocator::IsAllocated(const void* pointer) const
{
	// validate input
	ASSERT(pointer);

	BD* bd = user_list_head_;
	while (bd != nullptr)
	{
		// check if the pointer passed in exists within each descriptor
		if ((static_cast<const uint8_t*>(pointer) >= bd->block_pointer_ && static_cast<const uint8_t*>(pointer) < (bd->block_pointer_ + bd->block_size_)))
		{
			return true;
		}
		bd = bd->next_;
	}
	return false;
}

const size_t BlockAllocator::GetLargestFreeBlockSize(const size_t alignment) const
{
	// alignment should be power of two!
	ASSERT((alignment & (alignment - 1)) == 0);

	size_t largest_size = 0;
	// loop the free list
	BD* bd = free_list_head_;
	while (bd != nullptr)
	{
		// check actual block size not user block size
		if (bd->block_size_ > largest_size)
		{
			largest_size = bd->block_size_;
		}
		bd = bd->next_;
	}

#ifdef BUILD_DEBUG
	return largest_size - alignment - DEFAULT_GUARDBAND_SIZE * 2;
#else
	return largest_size - alignment;
#endif
}

#ifdef BUILD_DEBUG
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
		for (BD* bd = free_list_head_; bd != nullptr; bd = bd->next_)
		{
			VERBOSE("BD.id:%d size:%zu", bd->id_, bd->block_size_);
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
		for (BD* bd = user_list_head_; bd != nullptr; bd = bd->next_)
		{
			VERBOSE("BD.id:%d size:%zu", bd->id_, bd->block_size_);
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