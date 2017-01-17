#include "Memory\FixedSizeAllocator.h"

// engine includes
#include "Assert\Assert.h"
#include "Data\BitArray.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorUtil.h"
#include "Memory\BlockAllocator.h"

namespace engine {
namespace memory {

// initialize static members
FixedSizeAllocator*							FixedSizeAllocator::available_allocators_[MAX_FIXED_SIZE_ALLOCATORS] = { nullptr };
FixedSizeAllocator::FSASort					FixedSizeAllocator::FSASorter;

#ifdef BUILD_DEBUG
uint8_t										FixedSizeAllocator::counter_ = 0;
#endif

FixedSizeAllocator::FixedSizeAllocator(void* memory, const size_t total_block_size, const size_t fixed_block_size, const size_t num_blocks, BlockAllocator* allocator) : block_(static_cast<uint8_t*>(memory)),
	total_block_size_(total_block_size),
	fixed_block_size_(fixed_block_size),
	num_blocks_(num_blocks),
	block_allocator_(allocator),
	block_state_(nullptr)
{
	// validate input
	ASSERT(block_);
	ASSERT(fixed_block_size_ > 0);
	ASSERT(num_blocks_ > 0);
	ASSERT(block_allocator_);

	// get the amount of memory required to create a bit array
	const size_t bit_array_memory_size = engine::data::BitArray::GetRequiredMemorySize(num_blocks);	

	// create the bit array at the end of the block
	uint8_t* bit_array_memory = block_ + total_block_size_ - bit_array_memory_size;
	block_state_ = engine::data::BitArray::Create(num_blocks_, bit_array_memory);
	
	// reduce the usable memory
	total_block_size_ -= bit_array_memory_size;

#ifdef BUILD_DEBUG
	id_ = FixedSizeAllocator::counter_++;
	memset(block_, CLEAN_FILL, total_block_size_);
	VERBOSE("FixedSizeAllocator-%d created with %zu blocks of size:%zu", id_, num_blocks_, fixed_block_size_);
#endif
}

FixedSizeAllocator* FixedSizeAllocator::Create(const size_t block_size, const size_t num_blocks, BlockAllocator* allocator)
{
	// validate input
	ASSERT(block_size);
	ASSERT(num_blocks);
	ASSERT(allocator);

#ifdef BUILD_DEBUG
	const size_t guardband_size = DEFAULT_GUARDBAND_SIZE;
	const size_t size_type = sizeof(size_t);
#else
	const size_t guardband_size = 0;
	const size_t size_type = 0;
#endif

	// calculate the amount of memory required to create an FSA
	// per block, the FSA adds an overhead of 12 (32-bit) to 16 (64-bit) bytes in debug mode
	size_t fsa_memory_size = sizeof(FixedSizeAllocator) + num_blocks * (block_size + size_type + guardband_size * 2) + engine::data::BitArray::GetRequiredMemorySize(num_blocks);

	// allocate memory
	void* memory = allocator->Alloc(fsa_memory_size);
	ASSERT(memory);

	// move up the address of the usable block
	uint8_t* fsa_memory = static_cast<uint8_t*>(memory);
	fsa_memory += sizeof(FixedSizeAllocator);
	fsa_memory_size -= sizeof(FixedSizeAllocator);

	// create the FSA
	FixedSizeAllocator* fsa = new (memory) FixedSizeAllocator(fsa_memory, fsa_memory_size, block_size, num_blocks, allocator);
	ASSERT(fsa);

	return fsa;
}

void FixedSizeAllocator::Destroy(FixedSizeAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	BlockAllocator* block_allocator = allocator->block_allocator_;

#ifdef BUILD_DEBUG
	// TODO: print *more* diagnostics
	size_t first_set_bit = -1;
	if (allocator->block_state_->GetFirstSetBit(first_set_bit))
	{
		LOG_ERROR("WARNING! Found %zu unfreed allocations in FixedSizeAllocator-%d with fixed_block_size:%zu", allocator->GetNumOustandingBlocks(), allocator->id_, allocator->fixed_block_size_);
	}

	uint8_t id = allocator->id_;
	VERBOSE("FixedSizeAllocator-%d destroyed", id);
#endif

	block_allocator->Free(allocator);
}

bool FixedSizeAllocator::IsFixedSizeAllocatorAvailable(FixedSizeAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	// search for the allocator
	for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
	{
		if (available_allocators_[i] == allocator)
		{
			return true;
		}
	}
	return false;
}

bool FixedSizeAllocator::AddFixedSizeAllocator(FixedSizeAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	bool space_available = false;
	// add the element at the first available position
	for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
	{
		if (available_allocators_[i])
		{
			// prevent two allocators from having the same block size
			ASSERT(available_allocators_[i]->GetBlockSize() != allocator->GetBlockSize());
		}
		else
		{
			available_allocators_[i] = allocator;
			space_available = true;
			break;
		}
	}

	// ensure we have space for one more allocator
	ASSERT(space_available);

	// sort the allocators in ascending order of the block sizes they maintain
	std::sort(available_allocators_, (available_allocators_ + MAX_FIXED_SIZE_ALLOCATORS - 1), FSASorter);

	return true;
}

bool FixedSizeAllocator::RemoveFixedSizeAllocator(FixedSizeAllocator* allocator)
{
	// validate input
	ASSERT(allocator);

	bool found = false;
	// search for the allocator in the list of registered allocators
	for (uint8_t i = 0; i < MAX_BLOCK_ALLOCATORS; ++i)
	{
		if (available_allocators_[i] == allocator)
		{
			available_allocators_[i] = nullptr;
			found = true;
			break;
		}
	}

	// ensure we found the allocator
	ASSERT(found);

	// sort the allocators in ascending order of the block sizes they maintain
	std::sort(available_allocators_, (available_allocators_ + MAX_FIXED_SIZE_ALLOCATORS - 1), FSASorter);

	return true;
}

#ifdef BUILD_DEBUG
bool FixedSizeAllocator::CheckMemoryOverwrite(const size_t bit_index) const
{
	// validate input
	ASSERT(bit_index >= 0 && bit_index < num_blocks_);

	// get this block's starting address
	uint8_t* block = GetPointerForBlock(bit_index);

	// extract the size of this block as was requested by a user
	const size_t user_size = *reinterpret_cast<size_t*>(block);
	// move the block ahead
	block += sizeof(size_t);

	uint8_t lower_byte_counter = 0, upper_byte_counter = 0;
	for (uint8_t i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		// check lower guardband
		lower_byte_counter += (*(block + i) == GUARDBAND_FILL) ? 1 : 0;

		// check upper guardband
		upper_byte_counter += (*(block + DEFAULT_GUARDBAND_SIZE + user_size + i) == GUARDBAND_FILL) ? 1 : 0;
	}

	bool found_overwrite = !(lower_byte_counter >= DEFAULT_GUARDBAND_SIZE && upper_byte_counter >= DEFAULT_GUARDBAND_SIZE);
	if (found_overwrite)
	{
		LOG_ERROR("FixedSizeAllocator-%d with fixed_block_size_=%zu detected overwritten memory!", id_, fixed_block_size_);
	}

	return found_overwrite;
}
#endif

void* FixedSizeAllocator::Alloc()
{
	return Alloc(fixed_block_size_);
}

void* FixedSizeAllocator::Alloc(const size_t size)
{
	// validate input
	ASSERT(size <= fixed_block_size_);

	// check if there are any blocks available
	size_t bit_index = -1;
	bool block_available = block_state_->GetFirstClearBit(bit_index);

	// return nullptr if nothing was available
	if (!block_available)
	{
#ifdef BUILD_DEBUG
		LOG_ERROR("FixedSizeAllocator-%d with fixed_block_size_=%zu ran out of memory!", id_, fixed_block_size_);
#else
		LOG_ERROR("A FixedSizeAllocator ran out of memory!");
#endif
		return nullptr;
	}

	// set the bit at this index
	block_state_->SetBit(bit_index);

#ifdef BUILD_DEBUG
	const size_t guardband_size = DEFAULT_GUARDBAND_SIZE;
	const size_t size_type = sizeof(size_t);
#else
	const size_t guardband_size = 0;
	const size_t size_type = 0;
#endif

	// calculate the block's address
	uint8_t* block = block_ + bit_index * (guardband_size * 2 + size_type + fixed_block_size_);

#ifdef BUILD_DEBUG
	// cast to size_t* is needed since the block is of type uint8_t*
	size_t* block_size_t = reinterpret_cast<size_t*>(block);
	// save the size of this block
	*(block_size_t) = size;

	// clear the block
	memset(block + size_type + guardband_size, CLEAN_FILL, fixed_block_size_);

	// add guardbands
	for (uint8_t i = 0; i < guardband_size; ++i)
	{
		*(block + size_type + i) = GUARDBAND_FILL;
		*(block + size_type + guardband_size + size + i) = GUARDBAND_FILL;
	}
#endif

	return (block + size_type + guardband_size);
}

bool FixedSizeAllocator::Free(void* pointer)
{
	// validate input
	ASSERT(pointer != nullptr);

	// return if this allocator does not contain this pointer
	if (!Contains(pointer))
	{
		return false;
	}

#ifdef BUILD_DEBUG
	const size_t guardband_size = DEFAULT_GUARDBAND_SIZE;
	const size_t size_type = sizeof(size_t);
#else
	const size_t guardband_size = 0;
	const size_t size_type = 0;
#endif

	// get a pointer to a block
	uint8_t* block = static_cast<uint8_t*>(pointer) - guardband_size - size_type;

	// check if we recognize this pointer
	if ((block - block_) % (fixed_block_size_ + size_type + guardband_size * 2))
	{
#ifdef BUILD_DEBUG
		LOG_ERROR("FixedSizeAllocator-%d could not find pointer=%p passed into Free...bad adress!", id_, pointer);
#else
		LOG_ERROR("Bad input passed to FixedSizeAllocator::Free!");
#endif
		return false;
	}

	// calculate the index of the bit that represents this block
	size_t bit_index = (block - block_) / (fixed_block_size_ + size_type + guardband_size * 2);

	// validate bit_index
	if (bit_index < 0 || bit_index >= num_blocks_)
	{
#ifdef BUILD_DEBUG
		LOG_ERROR("FixedSizeAllocator-%d could not find pointer=%p passed into Free...couldn't map to an index!", id_, pointer);
#else
		LOG_ERROR("Bad input passed to FixedSizeAllocator::Free!");
#endif
		return false;
	}

	// check if this block is currently allocated
	if (block_state_->IsBitClear(bit_index))
	{
#ifdef BUILD_DEBUG
		LOG_ERROR("FixedSizeAllocator-%d could not free pointer=%p since it is not currently allocated!", id_, pointer);
#else
		LOG_ERROR("Bad input passed to FixedSizeAllocator::Free!");
#endif
		return false;
	}

#ifdef BUILD_DEBUG
	// check for overwrites
	ASSERT(!CheckMemoryOverwrite(bit_index));

	// clear the block
	memset(block, DEAD_FILL, fixed_block_size_ + size_type + guardband_size * 2);
#endif

	// clear the bit at this index
	block_state_->ClearBit(bit_index);

	return true;
}

bool FixedSizeAllocator::IsAllocated(const void* pointer) const
{
	// validate input
	ASSERT(pointer);

	// return if this allocator does not contain this pointer
	if (!Contains(pointer))
	{
		return false;
	}

#ifdef BUILD_DEBUG
	const size_t guardband_size = DEFAULT_GUARDBAND_SIZE;
	const size_t size_type = sizeof(size_t);
#else
	const size_t guardband_size = 0;
	const size_t size_type = 0;
#endif

	// get a pointer to a block
	const uint8_t* block = static_cast<const uint8_t*>(pointer) - guardband_size - size_type;

	// check if we recognize this pointer
	if ((block - block_) % (fixed_block_size_ + size_type + guardband_size * 2))
	{
		return false;
	}

	// calculate the index of the bit that represents this block
	size_t bit_index = (block - block_) / (fixed_block_size_ + size_type + guardband_size * 2);

	// validate bit_index
	if (bit_index < 0 || bit_index >= num_blocks_)
	{
		return false;
	}

	// check if this block is currently unallocated
	if (block_state_->IsBitClear(bit_index))
	{
		return false;
	}

	return true;
}

} // namespace memory
} // namespace engine