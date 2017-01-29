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

	void* DoAlloc(size_t i_size, const char* i_function_name);
	void DoFree(void* i_pointer, const char* i_function_name);

} // namespace memory
} // namespace engine

/*_Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(i_size)
_ACRTIMP _CRTALLOCATOR _CRT_JIT_INTRINSIC _CRTRESTRICT
void* __cdecl malloc(_In_ _CRT_GUARDOVERFLOW size_t i_size);

_ACRTIMP
void __cdecl free(_Pre_maybenull_ _Post_invalid_ void* i_pointer);*/

void* operator new(size_t i_size);
void operator delete(void* i_pointer);

void* operator new[](size_t i_size);
void operator delete[](void* i_pointer);

void* operator new(size_t i_size, engine::memory::AlignmentType i_alignment);
void operator delete(void* i_pointer, engine::memory::AlignmentType i_alignment);

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator);
void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator);

void* operator new[](size_t i_size, engine::memory::BlockAllocator* i_allocator);
void operator delete[](void* i_pointer, engine::memory::BlockAllocator* i_allocator);

void* operator new(size_t i_size, engine::memory::FixedSizeAllocator* i_allocator);
void operator delete(void* i_pointer, engine::memory::FixedSizeAllocator* i_allocator);

void* operator new[](size_t i_size, engine::memory::FixedSizeAllocator* i_allocator);
void operator delete[](void* i_pointer, engine::memory::FixedSizeAllocator* i_allocator);

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment);
void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment);

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t i_size, const char* i_file_name, unsigned int i_line);
void operator delete(void* i_pointer, const char* i_file_name, unsigned int i_line);
#else
#define TRACK_NEW
#endif

#endif // CUSTOM_NEW_H_