#ifdef ENABLE_TESTS

// engine includes
#include "Logger\Logger.h"

/************************ MEMORY TESTS ************************/
#ifdef ENABLE_ALLOCATOR_TEST
#include "Tests\BlockAllocatorTest.h"

bool HeapManager_UnitTest();

void TestFixedSizeAllocator();
#endif // ENABLE_ALLOCATOR_TEST

/************************ ENABLE OTHER TESTS ************************/
//#define ENABLE_VECTOR_CONST_TEST
//#define ENABLE_FLOAT_VALIDITY_TEST
//#define ENABLE_BIT_ARRAY_TEST
//#define ENABLE_STRONG_POINTER_TEST
//#define ENABLE_STRING_POOL_TEST
//#define ENABLE_JOB_SYSTEM_TEST
//#define ENABLE_MAT44_TEST
//#define ENABLE_FAST_MATH_TEST

#ifdef ENABLE_VECTOR_CONST_TEST
void TestVectorConstness();
#endif // ENABLE_VECTOR_CONST_TEST

#ifdef ENABLE_FLOAT_VALIDITY_TEST
void TestFloatValidity();
#endif // ENABLE_FLOAT_VALIDITY_TEST

#ifdef ENABLE_BIT_ARRAY_TEST
void RunBitArray_UnitTest();
#endif // ENABLE_BIT_ARRAY_TEST

#ifdef ENABLE_STRONG_POINTER_TEST
void TestSmartPointers();
#endif // ENABLE_STRONG_POINTER_TEST

#ifdef ENABLE_STRING_POOL_TEST
void TestStringPool();
#endif // ENABLE_STRING_POOL_TEST

#ifdef ENABLE_JOB_SYSTEM_TEST
void TestJobSystem();
#endif

#ifdef ENABLE_MAT44_TEST
void TestMat44();
#endif

#ifdef ENABLE_FAST_MATH_TEST
void TestFastMath();
#endif

/************************ RUN TESTS ************************/
void RunTests()
{
#ifdef ENABLE_VECTOR_CONST_TEST
    LOG("\n");
    TestVectorConstness();
#endif // ENABLE_VECTOR_CONST_TEST

#ifdef ENABLE_FLOAT_VALIDITY_TEST
    LOG("\n");
    TestFloatValidity();
#endif // ENABLE_FLOAT_VALIDITY_TEST
    
#ifdef ENABLE_BIT_ARRAY_TEST
    LOG("\n");
    RunBitArray_UnitTest();
#endif // ENABLE_BIT_ARRAY_TEST

#ifdef ENABLE_STRONG_POINTER_TEST
    LOG("\n");
    TestSmartPointers();
#endif // ENABLE_STRONG_POINTER_TEST

#ifdef ENABLE_STRING_POOL_TEST
    LOG("\n");
    TestStringPool();
#endif // ENABLE_STRING_POOL_TEST

#ifdef ENABLE_JOB_SYSTEM_TEST
    LOG("\n");
    TestJobSystem();
#endif // ENABLE_JOB_SYSTEM_TEST

#ifdef ENABLE_MAT44_TEST
    LOG("\n");
    TestMat44();
#endif // ENABLE_MAT44_TEST

#ifdef ENABLE_FAST_MATH_TEST
    LOG("\n");
    TestFastMath();
#endif // ENABLE_FAST_MATH_TEST

#ifdef ENABLE_ALLOCATOR_TEST
    LOG("\n");
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
}

#endif // ENABLE_TESTS