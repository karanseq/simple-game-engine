#include "Memory\AllocatorOverrides.h"

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

//_Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(size)
///*_ACRTIMP*/ _CRTALLOCATOR _CRT_JIT_INTRINSIC _CRTRESTRICT
//void* __cdecl malloc(_In_ _CRT_GUARDOVERFLOW size_t size)
//{
//	return engine::memory::DoAlloc(size, __FUNCTION__);
//}
//
///*_ACRTIMP*/
//void __cdecl free(_Pre_maybenull_ _Post_invalid_ void* pointer)
//{
//	engine::memory::DoFree(pointer, __FUNCTION__);
//}

void* operator new(size_t size)
{
	return engine::memory::DoAlloc(size, __FUNCTION__);
}

void operator delete(void* pointer)
{
	engine::memory::DoFree(pointer, __FUNCTION__);
}

void* operator new[](size_t size)
{
	return engine::memory::DoAlloc(size, __FUNCTION__);
}

void operator delete[](void* pointer)
{
	engine::memory::DoFree(pointer, __FUNCTION__);
}

void* operator new(size_t size, engine::memory::AlignmentType alignment)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	void* pointer = default_allocator->Alloc(size, alignment);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(size = %zu, alignemnt = %d) on the default allocator", __FUNCTION__, size, alignment);
#endif
	return pointer;
}

void operator delete(void* pointer, engine::memory::AlignmentType alignment)
{
	ASSERT(pointer);
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	default_allocator->Free(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(pointer = %p, alignemnt = %d) on the default allocator", __FUNCTION__, pointer, alignment);
#endif
}

void* operator new(size_t size, engine::memory::BlockAllocator* allocator)
{
	ASSERT(allocator);
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(size = %zu, allocator = %p) on BlockAllocator-%d", __FUNCTION__, size, allocator, allocator->GetID());
#endif
	return pointer;
}

void operator delete(void* pointer, engine::memory::BlockAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
	allocator->Free(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(pointer = %p, allocator = %p) on BlockAllocator-%d", __FUNCTION__, pointer, allocator, allocator->GetID());
#endif
}

void* operator new[](size_t size, engine::memory::BlockAllocator* allocator)
{
	ASSERT(allocator);
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(size = %zu, allocator = %p) on BlockAllocator-%d", __FUNCTION__, size, allocator, allocator->GetID());
#endif
	return pointer;
}

void operator delete[](void* pointer, engine::memory::BlockAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
	allocator->Free(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(pointer = %p, allocator = %p) on BlockAllocator-%d", __FUNCTION__, pointer, allocator, allocator->GetID());
#endif
}

void* operator new(size_t size, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(allocator);
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(size = %zu, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, size, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
	return pointer;
}

void operator delete(void* pointer, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
	allocator->Free(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(pointer = %p, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, pointer, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
}

void* operator new[](size_t size, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(allocator);
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(size = %zu, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, size, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
	return pointer;
}

void operator delete[](void* pointer, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
	allocator->Free(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(pointer = %p, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", __FUNCTION__, pointer, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
}

void* operator new(size_t size, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment)
{
	ASSERT(allocator);
	void* pointer = allocator->Alloc(size, alignment);
	ASSERT(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(size = %zu, allocator = %p, alignment = %d) on allocator-%d", __FUNCTION__, size, allocator, alignment, allocator->GetID());
#endif
	return pointer;
}

void operator delete(void* pointer, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment)
{
	ASSERT(pointer);
	ASSERT(allocator);
	allocator->Free(pointer);
#ifdef BUILD_DEBUG
	VERBOSE("Called %s(pointer = %p, allocator = %p, alignment = %d) on allocator-%d", __FUNCTION__, pointer, allocator, alignment, allocator->GetID());
#endif
}

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t size, const char* file_name, unsigned int line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	void* pointer = default_allocator->Alloc(size);
	ASSERT(pointer);
	VERBOSE("Called %s(size = %zu, file_name = %s, line = %d) on allocator-%d", __FUNCTION__, size, file_name, line, default_allocator->GetID());
	return pointer;
}

void operator delete(void* pointer, const char* file_name, unsigned int line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	default_allocator->Free(pointer);
	VERBOSE("Called %s(pointer = %p, file_name = %s, line = %d) on allocator-%d", __FUNCTION__, pointer, file_name, line, default_allocator->GetID());
}
#endif

namespace engine {
namespace memory {

	void* DoAlloc(size_t size, const char* function_name)
	{
		void* pointer = nullptr;

		// loop over the available fixed size allocators to find the best fit
		engine::memory::FixedSizeAllocator** const available_fsas = engine::memory::FixedSizeAllocator::GetAvailableFixedSizeAllocators();
		for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
		{
			// if the FSA exists and is big enough to service this request
			if (available_fsas[i] && available_fsas[i]->GetBlockSize() >= size)
			{
				pointer = available_fsas[i]->Alloc(size);
				if (pointer)
				{
	#ifdef BUILD_DEBUG
					VERBOSE("Called %s(size = %zu) on FixedSizeAllocator-%d with fixed_block_size:%zu", function_name, size, available_fsas[i]->GetID(), available_fsas[i]->GetBlockSize());
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

		pointer = default_allocator->Alloc(size);
		ASSERT(pointer);

	#ifdef BUILD_DEBUG
		VERBOSE("Called %s(size = %zu) on BlockAllocator-%d", function_name, size, default_allocator->GetID());
	#endif

		return pointer;
	}

	void DoFree(void* pointer, const char* function_name)
	{
		ASSERT(pointer);

		// get all available fixed size allocators
		engine::memory::FixedSizeAllocator** const fixed_size_allocators = engine::memory::FixedSizeAllocator::GetAvailableFixedSizeAllocators();

		// free the pointer from the appropriate allocator
		uint8_t num_fixed_size_allocators = MAX_FIXED_SIZE_ALLOCATORS;
		while (num_fixed_size_allocators > 0)
		{
			if (fixed_size_allocators[num_fixed_size_allocators - 1] && fixed_size_allocators[num_fixed_size_allocators - 1]->Free(pointer))
			{
	#ifdef BUILD_DEBUG
				VERBOSE("Called %s(pointer = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", function_name, pointer, fixed_size_allocators[num_fixed_size_allocators - 1]->GetID(), fixed_size_allocators[num_fixed_size_allocators - 1]->GetBlockSize());
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
			if (block_allocators[num_block_allocators - 1] && block_allocators[num_block_allocators - 1]->Free(pointer))
			{
	#ifdef BUILD_DEBUG
				VERBOSE("Called %s(pointer = %p) on BlockAllocator-%d", function_name, pointer, block_allocators[num_block_allocators - 1]->GetID());
	#endif
				return;
			}
			--num_block_allocators;
		}

		// this means the pointer could not be deleted
		LOG_ERROR("Could not %s(pointer = %p) on any of the allocators!", function_name, pointer);
	}

} // namespace memory
} // namespace engine
