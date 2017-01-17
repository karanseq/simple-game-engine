#ifndef CUSTOM_NEW_H_
#define CUSTOM_NEW_H_

// library includes
//#include <corecrt.h>
//#include <corecrt_malloc.h>

namespace engine {
namespace memory {

	/*
		AlignmentType
		- An enum that defines the various types of alignment supported by the overloaded new operator
	*/
	enum AlignmentType
	{
		ALIGNMENT_DEFAULT = 4,
		ALIGNMENT_8 = 8,
		ALIGNMENT_16 = 16,
		ALIGNMENT_32 = 32,
		ALIGNMENT_64 = 64
	};

	// forward declarations
	class BlockAllocator;
	class FixedSizeAllocator;

	void* DoAlloc(size_t size, const char* function_name);
	void DoFree(void* pointer, const char* function_name);

} // namespace memory
} // namespace engine

/*_Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(size)
_ACRTIMP _CRTALLOCATOR _CRT_JIT_INTRINSIC _CRTRESTRICT
void* __cdecl malloc(_In_ _CRT_GUARDOVERFLOW size_t size);

_ACRTIMP
void __cdecl free(_Pre_maybenull_ _Post_invalid_ void* pointer);*/

void* operator new(size_t size);
void operator delete(void* pointer);

void* operator new[](size_t size);
void operator delete[](void* pointer);

void* operator new(size_t size, engine::memory::AlignmentType alignment);
void operator delete(void* pointer, engine::memory::AlignmentType alignment);

void* operator new(size_t size, engine::memory::BlockAllocator* allocator);
void operator delete(void* pointer, engine::memory::BlockAllocator* allocator);

void* operator new[](size_t size, engine::memory::BlockAllocator* allocator);
void operator delete[](void* pointer, engine::memory::BlockAllocator* allocator);

void* operator new(size_t size, engine::memory::FixedSizeAllocator* allocator);
void operator delete(void* pointer, engine::memory::FixedSizeAllocator* allocator);

void* operator new[](size_t size, engine::memory::FixedSizeAllocator* allocator);
void operator delete[](void* pointer, engine::memory::FixedSizeAllocator* allocator);

void* operator new(size_t size, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment);
void operator delete(void* pointer, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment);

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t size, const char* file_name, unsigned int line);
void operator delete(void* pointer, const char* file_name, unsigned int line);
#else
#define TRACK_NEW
#endif

#endif // CUSTOM_NEW_H_