#ifdef BUILD_DEBUG

// library includes
#include <assert.h>
#include <algorithm>
#include <vector>

// engine includes
#include "Memory\BlockAllocator.h"
#include "Logger\Logger.h"

#define TEST_SINGLE_LARGE_ALLOCATION
#define __TRACK_ALLOCATIONS

bool HeapManager_UnitTest()
{
    LOG("-------------------- Running HeapManager_UnitTest --------------------");
    using namespace engine::memory;

    const size_t        sizeHeap = 1024 * 1024;
    const unsigned int  numDescriptors = 2048;

    // Allocate memory for my test heap.
    void * pHeapMemory = _aligned_malloc( sizeHeap, 4 );
    assert( pHeapMemory );

    // Create a heap manager for my test heap.
    //BlockAllocator * pHeapManager = BlockAllocator::Create(sizeHeap); //CreateHeapManager( pHeapMemory, sizeHeap, numDescriptors );
    BlockAllocator* pHeapManager = BlockAllocator::Create(pHeapMemory, sizeHeap);
    assert( pHeapManager );

    if( pHeapManager == nullptr )
        return false;

#ifdef TEST_SINGLE_LARGE_ALLOCATION
    // This is a test I wrote to check to see if using the whole block if it was almost consumed by 
    // an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
    {
        LOG("Before allocating single large allocation:");
        //ShowFreeBlocks( pHeapManager );
        pHeapManager->PrintFreeDescriptors();

        size_t largestBeforeAlloc = pHeapManager->GetLargestFreeBlockSize();
        //void * pPtr = alloc( pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave );
        void* pPtr = pHeapManager->Alloc(largestBeforeAlloc);

        if( pPtr )
        {
            LOG("After allocating single large allocation:");
            //ShowFreeBlocks( pHeapManager );
            pHeapManager->PrintFreeDescriptors();
            //printf( "\n" );
            LOG("");
#ifdef __TRACK_ALLOCATIONS
            //ShowOutstandingAllocations( pHeapManager );
            pHeapManager->PrintUsedDescriptors();
#endif // __TRACK_ALLOCATIONS
            //printf( "\n" );
            LOG("");

            size_t largestAfterAlloc = pHeapManager->GetLargestFreeBlockSize();
            //free( pHeapManager, pPtr );
            pHeapManager->Free(pPtr);

            LOG("After deallocating single large allocation:");
            //ShowFreeBlocks( pHeapManager );
            pHeapManager->PrintFreeDescriptors();
#ifdef __TRACK_ALLOCATIONS
            //ShowOutstandingAllocations( pHeapManager );
            pHeapManager->PrintUsedDescriptors();
#endif // __TRACK_ALLOCATIONS
            //printf( "\n" );
            LOG("");

            //Collect( pHeapManager );
            pHeapManager->Defragment();

            LOG("After defragmenting post single large allocation:");
            //ShowFreeBlocks( pHeapManager );
            pHeapManager->PrintFreeDescriptors();
#ifdef __TRACK_ALLOCATIONS
            //ShowOutstandingAllocations( pHeapManager );
            pHeapManager->PrintUsedDescriptors();
#endif // __TRACK_ALLOCATIONS
            //printf( "\n" );
            LOG("");

            size_t largestAfterCollect = pHeapManager->GetLargestFreeBlockSize();
        }
    }
#endif

    LOG("Beginning to exhaust allocations...");

    std::vector<void *> AllocatedAddresses;

    long    numAllocs = 0;
    long    numFrees = 0;
    long    numCollects = 0;

    bool    done = false;

    // allocate memory of random sizes up to 1024 bytes from the heap manager
    // until it runs out of memory
    do
    {
        const size_t        maxTestAllocationSize = 1024;

        const unsigned int  alignments[] = { 4 , 8, 16, 32, 64 };

        unsigned int    index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

        unsigned int    alignment = alignments[index];

        size_t          sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

        //void * pPtr = alloc( pHeapManager, sizeAlloc, alignment );
        LOG("ALLOC size:%zu alignment:%zu", sizeAlloc, alignment);
        void* pPtr = pHeapManager->Alloc(sizeAlloc, alignment);

#ifdef BUILD_DEBUG
        //pHeapManager->PrintAllDescriptors();
#endif

        assert( (reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0 );

        if( pPtr == nullptr )
        {
            //Collect( pHeapManager);
            pHeapManager->Defragment();

            LOG("ALLOC size:%zu alignment:%zu", sizeAlloc, alignment);
            //pPtr = alloc( pHeapManager, sizeAlloc, alignment );
            pPtr = pHeapManager->Alloc(sizeAlloc, alignment);

#ifdef BUILD_DEBUG
            //pHeapManager->PrintAllDescriptors();
#endif

            if( pPtr == nullptr )
            {
                done = true;
                break;
            }
        }

        AllocatedAddresses.push_back( pPtr );
        numAllocs++;

        const unsigned int freeAboutEvery = 10;
        const unsigned int garbageCollectAboutEvery = 40;

        if( !AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0) )
        {
            void * pPtr = AllocatedAddresses.back();
            AllocatedAddresses.pop_back();

            //bool success = free( pHeapManager, pPtr );
            LOG("FREE");
            bool success = pHeapManager->Free(pPtr);
            assert( success );

#ifdef BUILD_DEBUG
            //pHeapManager->PrintAllDescriptors();
#endif

            numFrees++;
        }

        if( (rand() % garbageCollectAboutEvery) == 0 )
        {
            //Collect( pHeapManager);
            LOG("DEFRAG");
            pHeapManager->Defragment();
            
            numCollects++;
        }

    } while( 1 );

    //printf( "After exhausting allocations:\n" );
    LOG("After exhausting allocations:");
    LOG("Num allocs:%ld\tNum frees:%ld\tNum collects:%ld", numAllocs, numFrees, numCollects);
    //ShowFreeBlocks( pHeapManager );
    pHeapManager->PrintFreeDescriptors();

#ifdef __TRACK_ALLOCATIONS
    //ShowOutstandingAllocations( pHeapManager );
    pHeapManager->PrintUsedDescriptors();
#endif // __TRACK_ALLOCATIONS
    //printf( "\n" );
    LOG("");

    // now free those blocks in a random order
    if( !AllocatedAddresses.empty() )
    {
        // randomize the addresses
        std::random_shuffle( AllocatedAddresses.begin(), AllocatedAddresses.end() );

        // return them back to the heap manager
        while( !AllocatedAddresses.empty() )
        {
            void * pPtr = AllocatedAddresses.back();
            AllocatedAddresses.pop_back();

            //bool success = Contains( pHeapManager, pPtr );
            bool success = pHeapManager->Contains(pPtr);
            assert( success );

            //success = IsAllocated( pHeapManager, pPtr );
            success = pHeapManager->IsAllocated(pPtr);
            assert( success );

            //success = free( pHeapManager, pPtr );
            success = pHeapManager->Free(pPtr);
            assert( success );
        }

        //printf( "After freeing allocations:\n" );
        LOG("After freeing allocations:");
        
        //ShowFreeBlocks( pHeapManager );
        pHeapManager->PrintFreeDescriptors();
#ifdef __TRACK_ALLOCATIONS
        //ShowOutstandingAllocations( pHeapManager );
        pHeapManager->PrintUsedDescriptors();
#endif // __TRACK_ALLOCATIONS

        // do garbage collection
        //Collect( pHeapManager );
        pHeapManager->Defragment();
        // our heap should be one single block, all the memory it started with

        //printf( "After garbage collection:\n" );
        LOG("After garbage collection:");

        //ShowFreeBlocks( pHeapManager );
        pHeapManager->PrintFreeDescriptors();
#ifdef __TRACK_ALLOCATIONS
        //ShowOutstandingAllocations( pHeapManager );
        pHeapManager->PrintUsedDescriptors();
#endif // __TRACK_ALLOCATIONS

        //printf( "\n" );       // do a large test allocation to see if garbage collection worked
        LOG("");
        //void * pPtr = alloc( pHeapManager, sizeHeap / 2 );
        void* pPtr = pHeapManager->Alloc(sizeHeap / 2);
        assert( pPtr );

        if( pPtr )
        {
            //bool success = free( pHeapManager, pPtr );
            bool success = pHeapManager->Free(pPtr);
            assert( success );

        }
    }

    //Destroy( pHeapManager );
    BlockAllocator::Destroy(pHeapManager);
    pHeapManager = nullptr;

    _aligned_free( pHeapMemory );

    LOG("-------------------- Finished HeapManager_UnitTest --------------------");

    // we succeeded
    return true;
}
#endif // BUILD_DEBUG