#include <stdint.h>

#include "Logger\Logger.h"
#include "Assert\Assert.h"
#include "Memory\BlockAllocator.h"
#include "Data\BitArray.h"

void TestBitArray()
{
	LOG("-------------------- Running Bit Array Test --------------------");

	engine::memory::BlockAllocator* allocator = engine::memory::BlockAllocator::GetDefaultAllocator();

	const size_t total_num_bits = 64;

	// create a BitArray passing in some memory
	void* bit_array_memory = allocator->Alloc(engine::data::BitArray::GetRequiredMemorySize(total_num_bits));
	engine::data::BitArray* bit_array = engine::data::BitArray::Create(total_num_bits, bit_array_memory, false);

	// test size
	LOG("bit_array->Size() = %zu", bit_array->Size());

	// test are all clear
	LOG("bit_array->ClearAll()");
	bit_array->ClearAll();
	LOG("Before setting individual bit, bit_array->AreAllClear() = %s", bit_array->AreAllClear() ? "true" : "false");

	// test set bit, check set bit & check clear bit
	bit_array->SetBit(9);
	bool is_set = bit_array->IsBitSet(9);
	bool is_clear = bit_array->IsBitClear(9);
	LOG("After setting individual bit, bit_array->AreAllClear() = %s", bit_array->AreAllClear() ? "true" : "false");

	// test are all set
	LOG("bit_array->SetAll()");
	bit_array->SetAll();
	LOG("Before clearing individual bit, bit_array->AreAllSet() = %s", bit_array->AreAllSet() ? "true" : "false");

	// test clear bit, check set bit & check clear bit
	bit_array->ClearBit(9);
	is_set = bit_array->IsBitSet(9);
	is_clear = bit_array->IsBitClear(9);
	LOG("After clearing individual bit, bit_array->AreAllSet() = %s", bit_array->AreAllSet() ? "true" : "false");

	LOG("bit_array->ClearAll()");
	bit_array->ClearAll();

	// test toggle bit
	size_t bit_index = 21;
	LOG("Before toggling individual bit, bit_array[%zu] = %s", bit_index, bit_array->Get(bit_index) ? "true" : "false");
	bit_array->ToggleBit(bit_index);
	LOG("After toggling individual bit, bit_array[%zu] = %s", bit_index, bit_array->Get(bit_index) ? "true" : "false");

	// test toggle all
	const size_t small_total_num_bits = 8;
	void* small_bit_array_memory = allocator->Alloc(engine::data::BitArray::GetRequiredMemorySize(small_total_num_bits));
	engine::data::BitArray* small_bit_array = engine::data::BitArray::Create(small_total_num_bits, small_bit_array_memory, false);

	small_bit_array->SetBit(1);
	small_bit_array->SetBit(3);
	small_bit_array->SetBit(5);
	small_bit_array->ToggleAll();
	LOG("small_bit_array->ToggleAll()");

	// test get first set bit
	bit_index = 13;
	LOG("Setting bit-%zu", bit_index);
	bit_array->SetBit(bit_index);
	
	bit_index = -1;
	bool success = bit_array->GetFirstSetBit(bit_index);
	LOG("After setting bit, bit_array->GetFirstSetBit(%zu) = %s", bit_index, success ? "true" : "false");
	
	bit_index = -1;
	success = bit_array->GetFirstClearBit(bit_index);
	LOG("After setting bit, bit_array->GetFirstClearBit(%zu) = %s", bit_index, success ? "true" : "false");

	LOG("bit_array->SetAll()");
	bit_array->SetAll();

	// test get first clear bit
	bit_index = 13;
	bit_array->ClearBit(bit_index);
	LOG("Clearing bit-%zu", bit_index);

	bit_index = -1;
	success = bit_array->GetFirstClearBit(bit_index);
	LOG("After setting bit, bit_array->GetFirstClearBit(%zu) = %s", bit_index, success ? "true" : "false");

	bit_index = -1;
	success = bit_array->GetFirstSetBit(bit_index);
	LOG("After setting bit, bit_array->GetFirstSetBit(%zu) = %s", bit_index, success ? "true" : "false");

	// test move constructor
	/*LOG("Calling move constructor...");
	engine::data::BitArray bit_array_move(std::move(bit_array_copy));
	LOG("bit_array[11] = %s", bit_array[11] ? "SET" : "CLEAR");
	LOG("bit_array[13] = %s", bit_array[13] ? "SET" : "CLEAR");

	// test move assignment operator
	LOG("Calling move assignment operator...");
	bit_array_move = std::move(bit_array_copy);
	LOG("bit_array[11] = %s", bit_array[11] ? "SET" : "CLEAR");
	LOG("bit_array[13] = %s", bit_array[13] ? "SET" : "CLEAR");*/
	
	allocator->Free(small_bit_array_memory);
	allocator->Free(bit_array_memory);

	LOG("-------------------- Finished Bit Array Test --------------------");
}
