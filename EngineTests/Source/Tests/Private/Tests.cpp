#ifdef ENABLE_TESTS

/************************ MEMORY TESTS ************************/
#ifdef ENABLE_ALLOCATOR_TEST
#include "Tests\BlockAllocatorTest.h"

// engine includes
#include "Logger\Logger.h"

bool HeapManager_UnitTest();

void TestFixedSizeAllocator();
#endif // ENABLE_ALLOCATOR_TEST

/************************ ENABLE OTHER TESTS ************************/
#define ENABLE_VECTOR_CONST_TEST
#define ENABLE_FLOAT_VALIDITY_TEST
#define ENABLE_BIT_ARRAY_TEST

#ifdef ENABLE_VECTOR_CONST_TEST
void TestVectorConstness();
#endif // ENABLE_VECTOR_CONST_TEST

#ifdef ENABLE_FLOAT_VALIDITY_TEST
void TestFloatValidity();
#endif // ENABLE_FLOAT_VALIDITY_TEST

#ifdef ENABLE_BIT_ARRAY_TEST
void RunBitArray_UnitTest();
#endif // ENABLE_BIT_ARRAY_TEST


/************************ RUN TESTS ************************/
void RunTests()
{
	LOG("-------------------- Running Tests --------------------");
	LOG("\n");
#ifdef ENABLE_VECTOR_CONST_TEST
	TestVectorConstness();
#endif // ENABLE_VECTOR_CONST_TEST

	LOG("\n");
#ifdef ENABLE_FLOAT_VALIDITY_TEST
	TestFloatValidity();
#endif // ENABLE_FLOAT_VALIDITY_TEST

	LOG("\n");
#ifdef ENABLE_BIT_ARRAY_TEST
	RunBitArray_UnitTest();
#endif // ENABLE_BIT_ARRAY_TEST

	LOG("\n");
#ifdef ENABLE_ALLOCATOR_TEST
	TestFixedSizeAllocator();
	LOG("\n");

#ifdef BUILD_DEBUG
		HeapManager_UnitTest();
#else
		const size_t mem_size = 1024 * 1024;
		BlockAllocatorTest::Init(mem_size);
		BlockAllocatorTest::RunTest00();
		BlockAllocatorTest::RunTest01();
		BlockAllocatorTest::RunTest02();
		BlockAllocatorTest::RunTest03();
		BlockAllocatorTest::Reset();
#endif // BUILD_DEBUG

#endif // ENABLE_ALLOCATOR_TEST

	LOG("-------------------- Finished Tests --------------------");
}

#endif // ENABLE_TESTS