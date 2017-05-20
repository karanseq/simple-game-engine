#include "Memory\AllocatorUtil.h"

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\AllocationCounter.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

namespace engine {
namespace memory {

void CreateAllocators()
{
    // initialize the default allocator
    BlockAllocator* default_allocator = BlockAllocator::GetDefaultAllocator();

    const size_t base_size = sizeof(size_t);

    // initialize the fixed size allocators
    // block size on 32-bit = 8 and on 64-bit = 16
    FixedSizeAllocator* fsa = FixedSizeAllocator::Create(base_size * 2, 600, default_allocator);
    FixedSizeAllocator::AddFixedSizeAllocator(fsa);

    // block size on 32-bit = 20 and on 64-bit = 40
    fsa = FixedSizeAllocator::Create(base_size * 5, 200, default_allocator);
    FixedSizeAllocator::AddFixedSizeAllocator(fsa);

    // block size on 32-bit = 36 and on 64-bit = 72
    fsa = FixedSizeAllocator::Create(base_size * 9, 300, default_allocator);
    FixedSizeAllocator::AddFixedSizeAllocator(fsa);

#ifdef BUILD_DEBUG
    // initialize the allocation counter
    AllocationCounter::Create();
#endif
}

void DestroyAllocators()
{
#ifdef BUILD_DEBUG
    AllocationCounter::Get()->Dump();
    AllocationCounter::Destroy();
#endif

    // destroy the fixed size allocators
    FixedSizeAllocator** const registered_fsas = FixedSizeAllocator::GetAvailableFixedSizeAllocators();

    for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
    {
        if (registered_fsas[i])
        {
            FixedSizeAllocator::Destroy(registered_fsas[i]);
            registered_fsas[i] = nullptr;
        }
    }

    // destroy the default allocator
    BlockAllocator::DestroyDefaultAllocator();
}

} // namespace memory
} // namespace engine