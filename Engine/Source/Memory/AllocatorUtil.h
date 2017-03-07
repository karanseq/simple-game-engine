#ifndef ENGINE_ALLOCATOR_UTIL_H_
#define ENGINE_ALLOCATOR_UTIL_H_

#include <stdint.h>

// global defines used across allocators
#define DEFAULT_BLOCK_SIZE                      1024 * 1024
#define DEFAULT_GUARDBAND_SIZE                  4
#define DEFAULT_BYTE_ALIGNMENT                  4
#define MAX_EXTRA_MEMORY                        8

#define GUARDBAND_FILL                          0xFD
#define DEAD_FILL                               0xDD
#define CLEAN_FILL                              0xCD

namespace engine {
namespace memory {

void CreateAllocators();
void DestroyAllocators();

#ifdef BUILD_DEBUG

struct AllocatorStatistics
{
    explicit AllocatorStatistics() : num_allocated(0),
        num_freed(0),
        num_outstanding(0),
        max_num_outstanding(0),
        allocated_memory_size(0),
        available_memory_size(0),
        max_allocated_memory_size(0)
    {}

    size_t                                  num_allocated;
    size_t                                  num_freed;
    size_t                                  num_outstanding;
    size_t                                  max_num_outstanding;
    size_t                                  allocated_memory_size;
    size_t                                  available_memory_size;
    size_t                                  max_allocated_memory_size;
};

#endif // BUILD_DEBUG

} // namespace memory
} // namespace engine

#endif // ENGINE_ALLOCATOR_UTIL_H_
