#ifndef ENGINE_ALLOCATOR_UTIL_H_
#define ENGINE_ALLOCATOR_UTIL_H_

#include <stdint.h>

// global defines used across allocators
#define DEFAULT_BLOCK_SIZE						1024 * 1024
#define DEFAULT_GUARDBAND_SIZE					4
#define DEFAULT_BYTE_ALIGNMENT					4
#define MAX_EXTRA_MEMORY						8

#define GUARDBAND_FILL							0xFD
#define DEAD_FILL								0xDD
#define CLEAN_FILL								0xCD

namespace engine {
namespace memory {

struct AllocatorStatistics
{
	explicit AllocatorStatistics() : total_allocated(0),
        total_freed(0),
        total_outstanding(0),
        max_outstanding(0)
    {}

    size_t                  total_allocated;
    size_t                  total_freed;
    size_t                  total_outstanding;
    size_t                  max_outstanding;
};

void CreateAllocators();
void DestroyAllocators();

} // namespace memory
} // namespace engine

#endif // ENGINE_ALLOCATOR_UTIL_H_
