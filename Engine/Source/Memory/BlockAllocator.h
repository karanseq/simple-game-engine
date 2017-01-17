#ifndef ENGINE_BLOCK_ALLOCATOR_H_
#define ENGINE_BLOCK_ALLOCATOR_H_

// library includes
#include <stdint.h>

// engine includes
#include "AllocatorUtil.h"

namespace engine {
namespace memory {

#define MAX_BLOCK_ALLOCATORS 5

/*
	BlockDescriptor
	- A struct that describes a block of memory managed by the block allocator
	- It contains a pointer to a block of memory as well as its size
	- It contains a pointer to the next & previous descriptor in a list or nullptr if its not part of a list
	- In debug mode, it contains an integer id that can be used to track memory leaks
*/
typedef struct BlockDescriptor
{
public:
	BlockDescriptor*		next_;					// pointer to the next block descriptor
	BlockDescriptor*		previous_;				// pointer to the previous block descriptor
	uint8_t*				block_pointer_;			// pointer to the actual block of data
	size_t					block_size_;			// size of the actual block of data

#ifdef BUILD_DEBUG
	size_t					user_size_;				// size of the block requested by the user
	uint32_t				id_;					// an identifier for each descriptor
#endif
} BD;

/*
	BlockAllocator
	- A simple block allocator that uses a linked list to keep track of allocations
	- It needs to be provided raw memory to operate on
	- It provides functions to allocate, free and defragment memory on demand
	- To allocate memory, users must pass in the desired size and desired byte alignment (defaults to 4-byte alignment)
	- In debug mode, it checks for memory overwrites by adding guardbands around the memory returned to the user
	- In debug mode, it provides functions to track allocations by uniquely identifying each descriptor to user memory
	- No more than 5 (MAX_BLOCK_ALLOCATORS) instances of this class must be created
	- In order to be within overridden versions of new, delete, malloc & free, an instance must be "registered" using
	  the static AddBlockAllocator function
*/
class BlockAllocator
{
private:
	// disable default constructor, copy constructor & assignment operator
	BlockAllocator() {}
	BlockAllocator(const BlockAllocator& copy);
	BlockAllocator& operator=(const BlockAllocator& ba);

	BlockAllocator(void* memory, size_t block_size);
	~BlockAllocator() {}

	void InitFirstBlockDescriptor();

	void AddToList(BD** head, BD** bd, bool enable_sort);
	void RemoveFromList(BD** head, BD** bd);

#ifdef BUILD_DEBUG
	bool CheckMemoryOverwrite(BD* bd) const;
	inline void ClearBlock(BD* bd, const unsigned char fill);
#endif

public:
	static BlockAllocator* Create(void* memory, size_t block_size);
	static void Destroy(BlockAllocator* allocator);

	static BlockAllocator* GetDefaultAllocator();
	static void CreateDefaultAllocator();
	static void DestroyDefaultAllocator();

	static bool IsBlockAllocatorAvailable(BlockAllocator* allocator);
	static bool AddBlockAllocator(BlockAllocator* allocator);
	static bool RemoveBlockAllocator(BlockAllocator* allocator);
	static inline BlockAllocator** const GetAvailableBlockAllocators();

	// Allocate a block of memory with given size & byte alignment
	void* Alloc(const size_t size, const size_t alignment = DEFAULT_BYTE_ALIGNMENT);
	// Deallocate a block of memory
	bool Free(void* pointer);

	// Run defragmentation
	void Defragment();

	// Query whether a given pointer is within this allocator's range
	inline bool Contains(const void* pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(const void* pointer) const;

	static inline size_t GetSizeOfBD();
	const size_t GetLargestFreeBlockSize(const size_t alignment = DEFAULT_BYTE_ALIGNMENT) const;
	inline const size_t GetTotalFreeMemorySize() const;

	inline const size_t GetNumOustandingBlocks() const;

#ifdef BUILD_DEBUG
	inline unsigned int GetID() const;
	void PrintAllDescriptors() const;
	void PrintFreeDescriptors() const;
	void PrintUsedDescriptors() const;
#endif

private:
	uint8_t*										block_;													// actual block of memory
	
	BD*												free_list_head_;										// list of block descriptors describing free blocks
	BD*												user_list_head_;										// list of block descriptors describing allocated blocks
	
	size_t											total_block_size_;										// total size of block
	static size_t									size_of_BD_;											// size of a BlockDescriptor object

#ifdef BUILD_DEBUG
	uint32_t										descriptor_counter_;									// a counter to keep track of all the descriptors (resets to 0 after reaching uint32_t's max value)
	uint8_t											id_;													// an id to keep track of this allocator in debug mode
	static uint8_t									counter_;												// a counter that will be used while setting ids for allocators
#endif

	static BlockAllocator*							available_allocators_[MAX_BLOCK_ALLOCATORS];			// an array of pointers to all registered block allocators

}; // class BlockAllocator

} // namespace memory
} // namespace engine

#include "BlockAllocator-inl.h"

#endif // ENGINE_BLOCK_ALLOCATOR_H_