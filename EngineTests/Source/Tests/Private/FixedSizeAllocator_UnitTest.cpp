// library includes
#include <vector>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

void ExhaustAllocator(engine::memory::FixedSizeAllocator* i_fsa)
{
	ASSERT(i_fsa);
	LOG("-------------------- Exhausting FixedSizeAllocator block_size:%zu num_blocks:%zu --------------------", i_fsa->GetBlockSize(), i_fsa->GetNumBlocks());
	std::vector<void*> allocations;

	// run first pass
	// make <num_blocks> allocs
	const size_t num_blocks = i_fsa->GetNumBlocks();
	for (size_t i = 0; i < num_blocks; ++i)
	{
		void* pointer = i_fsa->Alloc();
		ASSERT(pointer);
		allocations.push_back(pointer);
		LOG("Alloc-%zu num available blocks:%zu", i, i_fsa->GetNumAvailableBlocks());
	}

	ASSERT(!i_fsa->Alloc());
	ASSERT(i_fsa->GetNumOustandingBlocks() == num_blocks);
	ASSERT(i_fsa->GetNumAvailableBlocks() == 0);

	// free <num_blocks> in random order
	std::random_shuffle(allocations.begin(), allocations.end());
	for (size_t i = 0; i < num_blocks; ++i)
	{
		void* pointer = allocations.back();
		allocations.pop_back();

		bool success = i_fsa->Contains(pointer);
		assert(success);

		//success = IsAllocated( pHeapManager, pPtr );
		success = i_fsa->IsAllocated(pointer);
		assert(success);

		success = i_fsa->Free(pointer);
		assert(success);
		LOG("Free-%zu num outstanding blocks:%zu", i, i_fsa->GetNumOustandingBlocks());
	}

	ASSERT(i_fsa->GetNumAvailableBlocks() == num_blocks);
	ASSERT(i_fsa->GetNumOustandingBlocks() == 0);

	// run second pass
	size_t num_allocs = 0, num_frees = 0;
	do
	{
		const uint8_t free_every = 10;

		void* pointer = i_fsa->Alloc();
		if (!pointer)
		{
			break;
		}

		++num_allocs;
		memset(pointer, 65, i_fsa->GetBlockSize());

		if ((num_allocs % free_every) == 0)
		{
			++num_frees;
			i_fsa->Free(pointer);
			LOG("Free-%zu num outstanding blocks:%zu", num_frees, i_fsa->GetNumOustandingBlocks());
		}
		else
		{
			allocations.push_back(pointer);
		}

		LOG("Alloc-%zu num available blocks:%zu", num_allocs, i_fsa->GetNumAvailableBlocks());

	} while (true);

	ASSERT(i_fsa->GetNumOustandingBlocks() == num_blocks);
	ASSERT(i_fsa->GetNumAvailableBlocks() == 0);

	size_t unfreed_allocations = allocations.size();
	while (unfreed_allocations-- > 0)
	{
		void* pointer = allocations.back();
		allocations.pop_back();

		bool success = i_fsa->Contains(pointer);
		assert(success);

		//success = IsAllocated( pHeapManager, pPtr );
		success = i_fsa->IsAllocated(pointer);
		assert(success);

		success = i_fsa->Free(pointer);
		assert(success);
	}

	ASSERT(i_fsa->GetNumAvailableBlocks() == num_blocks);
	ASSERT(i_fsa->GetNumOustandingBlocks() == 0);

	LOG("-------------------- Finished exhausting FixedSizeAllocator block_size:%zu num_blocks:%zu --------------------", i_fsa->GetBlockSize(), i_fsa->GetNumBlocks());
}

void TestFixedSizeAllocator()
{
	LOG("-------------------- Running FixedSizeAllocator_UnitTest --------------------");
	engine::memory::BlockAllocator*				default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();

	engine::memory::FixedSizeAllocator*			fsa_48 = engine::memory::FixedSizeAllocator::Create(48, 100, default_allocator);
	ExhaustAllocator(fsa_48);
	engine::memory::FixedSizeAllocator::Destroy(fsa_48);
	LOG("-------------------- Finished FixedSizeAllocator_UnitTest --------------------");
}
