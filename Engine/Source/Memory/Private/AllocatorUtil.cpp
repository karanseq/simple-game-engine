#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

namespace engine {
namespace memory {

	void CreateAllocators()
	{
		// initialize the default allocator
		BlockAllocator* default_allocator = BlockAllocator::GetDefaultAllocator();

		// initialize the fixed size allocators
		FixedSizeAllocator* fsa = FixedSizeAllocator::Create(8, 32, default_allocator);
		FixedSizeAllocator::AddFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(16, 56, default_allocator);
		FixedSizeAllocator::AddFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(36, 24, default_allocator);
		FixedSizeAllocator::AddFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(76, 20, default_allocator);
		FixedSizeAllocator::AddFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(252, 20, default_allocator);
		FixedSizeAllocator::AddFixedSizeAllocator(fsa);
	}

	void DestroyAllocators()
	{
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