#ifdef ENABLE_ALLOCATOR_TEST

#ifndef ALLOCATOR_TEST_H_
#define ALLOCATOR_TEST_H_

#include "Memory/BlockAllocator.h"

class BlockAllocatorTest
{
private:
	BlockAllocatorTest() = delete;
	~BlockAllocatorTest() = delete;
	BlockAllocatorTest(const BlockAllocatorTest& i_copy) = delete;
	BlockAllocatorTest& operator=(const BlockAllocatorTest& i_at) = delete;

	static char* DoAlloc(const size_t i_size);
	static void DoFree(char* pointer, const size_t i_size);

public:
	static void Init(const size_t i_total_memory);
	static void Reset();

	static void RunTest00();
	static void RunTest01();
	static void RunTest02();
	static void RunTest03();

private:
	static void*									memory_;
	static engine::memory::BlockAllocator*			block_allocator_;
};

#endif // ALLOCATOR_TEST_H_

#endif // ENABLE_ALLOCATOR_TEST
