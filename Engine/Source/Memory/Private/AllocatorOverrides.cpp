#include "Memory\AllocatorOverrides.h"

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

//_Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(i_size)
///*_ACRTIMP*/ _CRTALLOCATOR _CRT_JIT_INTRINSIC _CRTRESTRICT
//void* __cdecl malloc(_In_ _CRT_GUARDOVERFLOW size_t i_size)
//{
//	return engine::memory::DoAlloc(i_size, __FUNCTION__);
//}
//
///*_ACRTIMP*/
//void __cdecl free(_Pre_maybenull_ _Post_invalid_ void* i_pointer)
//{
//	engine::memory::DoFree(i_pointer, __FUNCTION__);
//}

void* operator new(size_t i_size)
{
	return engine::memory::DoAlloc(i_size, __FUNCTION__);
}

void operator delete(void* i_pointer)
{
	engine::memory::DoFree(i_pointer, __FUNCTION__);
}

void* operator new[](size_t i_size)
{
	return engine::memory::DoAlloc(i_size, __FUNCTION__);
}

void operator delete[](void* i_pointer)
{
	engine::memory::DoFree(i_pointer, __FUNCTION__);
}

void* operator new(size_t i_size, engine::memory::AlignmentType i_alignment)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	void* pointer = default_allocator->Alloc(i_size, i_alignment);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_size = %zu, alignemnt = %d) on the default allocator", __FUNCTION__, i_size, i_alignment);
#endif
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::AlignmentType i_alignment)
{
	ASSERT(i_pointer);
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	default_allocator->Free(i_pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_pointer = %p, alignemnt = %d) on the default allocator", __FUNCTION__, i_pointer, i_alignment);
#endif
}

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_allocator);
	void* pointer = i_allocator->Alloc(i_size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_size = %zu, i_allocator = %p) on BlockAllocator-%d", __FUNCTION__, i_size, i_allocator, i_allocator->GetID());
#endif
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
	i_allocator->Free(i_pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_pointer = %p, i_allocator = %p) on BlockAllocator-%d", __FUNCTION__, i_pointer, i_allocator, i_allocator->GetID());
#endif
}

void* operator new[](size_t i_size, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_allocator);
	void* pointer = i_allocator->Alloc(i_size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_size = %zu, i_allocator = %p) on BlockAllocator-%d", __FUNCTION__, i_size, i_allocator, i_allocator->GetID());
#endif
	return pointer;
}

void operator delete[](void* i_pointer, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
	i_allocator->Free(i_pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_pointer = %p, i_allocator = %p) on BlockAllocator-%d", __FUNCTION__, i_pointer, i_allocator, i_allocator->GetID());
#endif
}

void* operator new(size_t i_size, engine::memory::FixedSizeAllocator* i_allocator)
{
	ASSERT(i_allocator);
	void* pointer = i_allocator->Alloc(i_size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_size = %zu, i_allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, i_size, i_allocator, i_allocator->GetID(), i_allocator->GetBlockSize());
#endif
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::FixedSizeAllocator* i_allocator)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
	i_allocator->Free(i_pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_pointer = %p, i_allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, i_pointer, i_allocator, i_allocator->GetID(), i_allocator->GetBlockSize());
#endif
}

void* operator new[](size_t i_size, engine::memory::FixedSizeAllocator* i_allocator)
{
	ASSERT(i_allocator);
	void* pointer = i_allocator->Alloc(i_size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_size = %zu, i_allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, i_size, i_allocator, i_allocator->GetID(), i_allocator->GetBlockSize());
#endif
	return pointer;
}

void operator delete[](void* i_pointer, engine::memory::FixedSizeAllocator* i_allocator)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
	i_allocator->Free(i_pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_pointer = %p, i_allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, i_pointer, i_allocator, i_allocator->GetID(), i_allocator->GetBlockSize());
#endif
}

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment)
{
	ASSERT(i_allocator);
	void* pointer = i_allocator->Alloc(i_size, i_alignment);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_size = %zu, i_allocator = %p, i_alignment = %d) on i_allocator-%d", __FUNCTION__, i_size, i_allocator, i_alignment, i_allocator->GetID());
#endif
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
	i_allocator->Free(i_pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(i_pointer = %p, i_allocator = %p, i_alignment = %d) on i_allocator-%d", __FUNCTION__, i_pointer, i_allocator, i_alignment, i_allocator->GetID());
#endif
}

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t i_size, const char* i_file_name, unsigned int i_line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	void* pointer = default_allocator->Alloc(i_size);
	ASSERT(pointer);
	VERBOSE("Called %s(i_size = %zu, i_file_name = %s, i_line = %d) on i_allocator-%d", __FUNCTION__, i_size, i_file_name, i_line, default_allocator->GetID());
	return pointer;
}

void operator delete(void* i_pointer, const char* i_file_name, unsigned int i_line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	default_allocator->Free(i_pointer);
	VERBOSE("Called %s(i_pointer = %p, i_file_name = %s, i_line = %d) on i_allocator-%d", __FUNCTION__, i_pointer, i_file_name, i_line, default_allocator->GetID());
}
#endif

namespace engine {
namespace memory {

	void* DoAlloc(size_t i_size, const char* i_function_name)
	{
		void* pointer = nullptr;

		// loop over the available fixed size allocators to find the best fit
		engine::memory::FixedSizeAllocator** const available_fsas = engine::memory::FixedSizeAllocator::GetAvailableFixedSizeAllocators();
		for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
		{
			// if the FSA exists and is big enough to service this request
			if (available_fsas[i] && available_fsas[i]->GetBlockSize() >= i_size)
			{
				pointer = available_fsas[i]->Alloc(i_size);
				if (pointer)
				{
	#ifdef BUILD_DEBUG
					VERBOSE("Called %s(i_size = %zu) on FixedSizeAllocator-%d with fixed_block_size:%zu", i_function_name, i_size, available_fsas[i]->GetID(), available_fsas[i]->GetBlockSize());
	#endif
					return pointer;
				}
				// at this point, we're choosing to try and allocate using the next available FSA.
				// we could choose to break out of this loop and allocate using the default block allocator instead.
			}
		}

		// service this request using the default block allocator
		engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
		ASSERT(default_allocator);

		pointer = default_allocator->Alloc(i_size);
		ASSERT(pointer);

	#ifdef BUILD_DEBUG
		VERBOSE("Called %s(i_size = %zu) on BlockAllocator-%d", i_function_name, i_size, default_allocator->GetID());
	#endif

		return pointer;
	}

	void DoFree(void* i_pointer, const char* i_function_name)
	{
		ASSERT(i_pointer);

		// get all available fixed size allocators
		engine::memory::FixedSizeAllocator** const fixed_size_allocators = engine::memory::FixedSizeAllocator::GetAvailableFixedSizeAllocators();

		// free the pointer from the appropriate allocator
		uint8_t num_fixed_size_allocators = MAX_FIXED_SIZE_ALLOCATORS;
		while (num_fixed_size_allocators > 0)
		{
			if (fixed_size_allocators[num_fixed_size_allocators - 1] && fixed_size_allocators[num_fixed_size_allocators - 1]->Free(i_pointer))
			{
	#ifdef BUILD_DEBUG
				VERBOSE("Called %s(i_pointer = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", i_function_name, i_pointer, fixed_size_allocators[num_fixed_size_allocators - 1]->GetID(), fixed_size_allocators[num_fixed_size_allocators - 1]->GetBlockSize());
	#endif
				return;
			}
			--num_fixed_size_allocators;
		}

		// get all available block allocators
		engine::memory::BlockAllocator** const block_allocators = engine::memory::BlockAllocator::GetAvailableBlockAllocators();

		// free the pointer from the appropriate allocator
		uint8_t num_block_allocators = MAX_BLOCK_ALLOCATORS;
		while (num_block_allocators > 0)
		{
			if (block_allocators[num_block_allocators - 1] && block_allocators[num_block_allocators - 1]->Free(i_pointer))
			{
	#ifdef BUILD_DEBUG
				VERBOSE("Called %s(i_pointer = %p) on BlockAllocator-%d", i_function_name, i_pointer, block_allocators[num_block_allocators - 1]->GetID());
	#endif
				return;
			}
			--num_block_allocators;
		}

		// this means the pointer could not be deleted
		LOG_ERROR("Could not %s(i_pointer = %p) on any of the allocators!", i_function_name, i_pointer);
	}

} // namespace memory
} // namespace engine
