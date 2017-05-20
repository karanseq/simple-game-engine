#include "Data\BitArray.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"

void BitArray_UnitTest(const size_t i_bitCount)
{
    using namespace engine::data;

    const size_t bit_array_memory_size = BitArray::GetRequiredMemorySize(i_bitCount);
    void* bit_array_memory = engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(bit_array_memory_size);

    BitArray* pMyArray = BitArray::Create(i_bitCount, bit_array_memory);

    size_t testBitIndex = 5 >= i_bitCount ? i_bitCount - 1 : 5;
    pMyArray->SetBit(testBitIndex);

    size_t firstSetBit = 0;
    size_t firstClearBit = 0;

    bool foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
    assert(foundSetBit && (firstSetBit == testBitIndex));

    pMyArray->ClearBit(testBitIndex);
    foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
    assert(foundSetBit == false);

    for (uint16_t i = 0; i < i_bitCount; i++)
    {
        assert(pMyArray->IsBitClear(i) == true);
        assert(pMyArray->IsBitSet(i) == false);

        size_t bit = 0;

        pMyArray->GetFirstClearBit(bit);
        assert(bit == i);

        pMyArray->SetBit(i);

        assert(pMyArray->IsBitClear(i) == false);
        assert(pMyArray->IsBitSet(i) == true);

        bool success = pMyArray->GetFirstClearBit(bit);
        assert(((i < (i_bitCount - 1)) && success && (bit == (i + 1))) || ((i == (i_bitCount - 1)) && !success));
    }

    //pMyArray->SetAllBits();
    pMyArray->SetAll();
    assert(pMyArray->GetFirstClearBit(firstClearBit) == false);

    //pMyArray->ClearAllBits();
    pMyArray->ClearAll();
    assert(pMyArray->GetFirstSetBit(firstSetBit) == false);

    //delete pMyArray;
    engine::memory::BlockAllocator::GetDefaultAllocator()->Free(pMyArray);
}

void RunBitArray_UnitTest()
{
    LOG("-------------------- Running BitArray_UnitTest --------------------");

    for (size_t i = 1; i < 65; ++i)
    {
        LOG("Testing with %zu bit/s...", i);
        BitArray_UnitTest(i);
    }

    size_t bit_count = 1000;
    LOG("Testing with %zu bits...", bit_count);
    BitArray_UnitTest(bit_count);

    LOG("-------------------- Finished BitArray_UnitTest --------------------");
}