#include "Data\BitArray.h"

// library includes
#include <intrin.h>
#include <string.h>

// engine includes
#include "Assert\Assert.h"
#include "Memory\AllocatorOverrides.h"
#include "Memory\AllocatorUtil.h"

namespace engine {
namespace data {

	const size_t BitArray::bit_depth_ = sizeof(size_t) * 8;

	BitArray::BitArray(size_t num_bits, void* memory, bool start_set) : buckets_(static_cast<size_t*>(memory)),
		num_buckets_(((num_bits & (bit_depth_ - 1)) ? 1 : 0) + num_bits / bit_depth_),
		num_bits_(num_bits)
	{
		ASSERT(buckets_);
		ASSERT(num_bits_ > 0);

		memset(buckets_, start_set ? ~0 : 0, sizeof(buckets_) * num_buckets_);
	}

	BitArray* BitArray::Create(size_t num_bits, void* memory, bool start_set)
	{
		ASSERT(num_bits > 0);
		ASSERT(memory);

		uint8_t* bit_array_memory = static_cast<uint8_t*>(memory);
		bit_array_memory += sizeof(BitArray);
		
		BitArray* bit_array = new (memory) BitArray(num_bits, bit_array_memory, start_set);
		ASSERT(bit_array);

		return bit_array;
	}
	
	BitArray::~BitArray()
	{}

	BitArray::BitArray(BitArray&& copy) : buckets_(copy.buckets_),
		num_buckets_(copy.num_buckets_),
		num_bits_(copy.num_bits_)
	{
		copy.buckets_ = nullptr;
	}

	bool BitArray::AreAllClear() const
	{
		size_t bit_index = 0;
		bool success = GetFirstSetBit(bit_index);
		
		// if we found no set bits at all, return true
		// if we found a set bit, return true if it was the last bit else return false
		return (!success || bit_index >= num_bits_);
	}

	bool BitArray::AreAllSet() const
	{
		size_t bit_index = 0;
		bool success = GetFirstClearBit(bit_index);

		// if we found no clear bits at all, return true
		// if we found a clear bit, return true if it was the last bit else return false
		return (!success || bit_index >= num_bits_);
	}

	void BitArray::SetBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / bit_depth_;

		// set the respective bit in the respective bucket
		buckets_[bucket_index] |= static_cast<size_t>(1) << bit_index;
	}

	void BitArray::ClearBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / bit_depth_;

		// set the respective bit in the respective bucket
		buckets_[bucket_index] &= ~(static_cast<size_t>(1) << bit_index);
	}

	void BitArray::ToggleBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / bit_depth_;

		buckets_[bucket_index] ^= static_cast<size_t>(1) << bit_index;
	}

	bool BitArray::GetFirstSetBit(size_t &bit_index) const
	{
		size_t bucket_index = 0;
		unsigned long bit_index_long = 0;

		// quick skip buckets where no bits are set
		while (buckets_[bucket_index] == 0 && bucket_index < num_buckets_)
		{
			++bucket_index;
		}

		// were all bits clear?
		if (bucket_index >= num_buckets_)
		{
			return false;
		}

#if defined(_WIN64)
		char is_non_zero = _BitScanForward64(&bit_index_long, buckets_[bucket_index]);
#else
		char is_non_zero = _BitScanForward(&bit_index_long, buckets_[bucket_index]);
#endif
		if (is_non_zero)
		{
			size_t new_bit_index = bucket_index * bit_depth_ + bit_index_long;
			if (new_bit_index < num_bits_)
			{
				bit_index = new_bit_index;
				return true;
			}
		}

		return false;
	}

	bool BitArray::GetFirstClearBit(size_t &bit_index) const
	{
		size_t bucket_index = 0;
		unsigned long bit_index_long = 0;

		// quick skip buckets where no bits are clear
		while (buckets_[bucket_index] == ~0 && bucket_index < num_buckets_)
		{
			++bucket_index;
		}

		// were all bits set?
		if (bucket_index >= num_buckets_)
		{
			return false;
		}

#if defined(_WIN64)
		char is_non_zero = _BitScanForward64(&bit_index_long, ~(buckets_[bucket_index]));
#else
		char is_non_zero = _BitScanForward(&bit_index_long, ~(buckets_[bucket_index]));
#endif
		if (is_non_zero)
		{
			size_t new_bit_index = bucket_index * bit_depth_ + bit_index_long;
			if (new_bit_index < num_bits_)
			{
				bit_index = new_bit_index;
				return true;
			}
		}

		return false;
	}	

} // namespace data
} // namespace engine
