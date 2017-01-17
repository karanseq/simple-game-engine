#ifdef ENABLE_ALLOCATOR_TEST

#include "Tests/BlockAllocatorTest.h"

// library includes
#include <stdlib.h>
#include <vector>		// required ONLY for test03

// engine includes
#include "Logger/Logger.h"

//#define SIMULATE_MEMORY_OVERWRITE

void*									BlockAllocatorTest::memory_ = nullptr;
engine::memory::BlockAllocator*			BlockAllocatorTest::block_allocator_ = nullptr;

void BlockAllocatorTest::Init(size_t i_total_memory)
{
	LOG("Testing BlockAllocator TOTAL_MEM:%zu", i_total_memory);

	memory_ = _aligned_malloc(i_total_memory, DEFAULT_BYTE_ALIGNMENT);

	block_allocator_ = engine::memory::BlockAllocator::Create(memory_, i_total_memory);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
}

void BlockAllocatorTest::Reset()
{
	LOG("Destroying BlockAllocator...");
	engine::memory::BlockAllocator::Destroy(block_allocator_);
	block_allocator_ = nullptr;

	_aligned_free(memory_);
	memory_ = nullptr;
}

char* BlockAllocatorTest::DoAlloc(const size_t i_size)
{
	LOG("Alloc-%zu", i_size);
	char* ret = static_cast<char*>(block_allocator_->Alloc(i_size));
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
	return ret;
}

void BlockAllocatorTest::DoFree(char* i_pointer, const size_t i_size)
{
	LOG("Free-%zu", i_size);
	block_allocator_->Free(i_pointer);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
}

void BlockAllocatorTest::RunTest00()
{
	LOG("-------------------- Running Test 00 --------------------");

	size_t		size = 200;
	size_t		alignment = 32;
	void*		pointer1 = block_allocator_->Alloc(size, alignment);
	LOG("Alloc-200 aligned:%d", reinterpret_cast<uintptr_t>(pointer1) % alignment);

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	size = 200;
	alignment = 16;
	void*		pointer2 = block_allocator_->Alloc(size, alignment);
	LOG("Alloc-200 aligned:%d", reinterpret_cast<uintptr_t>(pointer1) % alignment);

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	if (pointer1)			block_allocator_->Free(pointer1);
	if (pointer2)			block_allocator_->Free(pointer2);

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	block_allocator_->Defragment();

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 00 --------------------");
}

void BlockAllocatorTest::RunTest01()
{
	LOG("-------------------- Running Test 01 --------------------");

	const size_t		num_pointers = 25;
	char*				pointers[num_pointers] = { 0 };

	for (uint8_t i = 0; i < num_pointers; ++i)
	{
		const size_t	max_size = 512;
		size_t			rand_size = 1 + rand() % max_size;

		pointers[i] = static_cast<char*>(block_allocator_->Alloc(rand_size));

		const uint8_t	a_ascii = 65;
		if (pointers[i])
		{
			for (unsigned int j = 0; j < rand_size; ++j)
			{
				pointers[i][j] = a_ascii + i;
			}
		}
		else
		{
			break;
		}

#ifdef SIMULATE_MEMORY_OVERWRITE
		// write beyond what we requested
		if (i % 3)
		{
			pointers[i][rand_size] = a_ascii + i;
			pointers[i][rand_size + 1] = a_ascii + i;
		}
#endif
	}

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	for (uint8_t i = 0; i < num_pointers; ++i)
	{
		if (pointers[i] != nullptr)
		{
			LOG("pointer[%d] = %p : %s", i, pointers[i], pointers[i]);
		}
		else
		{
			LOG("pointer[%d] = nullptr", i);
		}
	}

	for (uint8_t i = 0; i < num_pointers; ++i)
	{
		LOG("Freeing %d...", i);
		block_allocator_->Free(pointers[i]);
		pointers[i] = nullptr;
	}

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 01 --------------------");
}

void BlockAllocatorTest::RunTest02()
{
	LOG("-------------------- Running Test 02 --------------------");

	size_t		size = 9;
	char*		buf1 = DoAlloc(size);
	size = 23;
	char*		buf2 = DoAlloc(size);
	
	size = 9;
	DoFree(buf1, size);
	size = 23;
	DoFree(buf2, size);

	size = 10;
	buf2 = DoAlloc(size);
	size = 22;
	buf1 = DoAlloc(size);

	size = 22;
	DoFree(buf1, size);
	size = 10;
	DoFree(buf2, size);

	block_allocator_->Defragment();
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 02 --------------------");
}

void BlockAllocatorTest::RunTest03()
{
	LOG("-------------------- Running Test 03 --------------------");
	const uint16_t			iterations = 512;
	const uint32_t			max_size = 1024 * 5;
	const uint8_t			success_chance = 3;

	std::vector<void*> unfreed_pointers;

	for (uint16_t i = 0; i < iterations; ++i)
	{
		const size_t		rand_size = static_cast<size_t>(rand() % max_size);
		LOG("Request-%d Alloc size:%zu", i, rand_size);

		void*				buf = block_allocator_->Alloc(rand_size);
		bool				successful = (buf != nullptr);

		if (successful && (rand() % 10) > success_chance)
		{
			LOG("Request-%d Free size:%zu", i, rand_size);
			block_allocator_->Free(buf);
		}
		else if (successful)
		{
			unfreed_pointers.push_back(buf);
		}

	}

	LOG("Finished %d iterations...", iterations);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	const size_t			num_unfreed_pointers = unfreed_pointers.size();
	LOG("Freeing %zu user allocations...", num_unfreed_pointers);

	for (uint16_t i = 0; i < num_unfreed_pointers; ++i)
	{
		block_allocator_->Free(unfreed_pointers[i]);
	}
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	block_allocator_->Defragment();
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 03 --------------------");
}

#endif // ENABLE_ALLOCATOR_TEST
