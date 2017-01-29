#include "BitArray.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace data {

	inline BitArray& BitArray::operator=(BitArray&& i_bit_array)
	{
		// check for self assignment
		if (this != &i_bit_array)
		{
			std::swap(buckets_, i_bit_array.buckets_);
			num_buckets_ = i_bit_array.num_buckets_;
			num_bits_ = i_bit_array.num_bits_;
		}
		return *this;
	}

	inline void BitArray::ClearAll()
	{
		memset(buckets_, 0, sizeof(buckets_) * num_buckets_);
	}

	inline void BitArray::SetAll()
	{
		memset(buckets_, ~0, sizeof(buckets_) * num_buckets_);
	}

	inline void BitArray::ToggleAll()
	{
		for (size_t i = 0; i < num_buckets_; ++i)
		{
			buckets_[i] = ~buckets_[i];
		}
	}

	inline bool BitArray::IsBitSet(size_t i_bit_index) const
	{
		// validate input
		ASSERT(i_bit_index >= 0);
		ASSERT(i_bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = i_bit_index / bit_depth_;

		return (buckets_[bucket_index] >> i_bit_index) & static_cast<size_t>(1);
	}

	inline bool BitArray::IsBitClear(size_t i_bit_index) const
	{
		return !IsBitSet(i_bit_index);
	}

	inline bool BitArray::Get(size_t i_bit_index) const
	{
		return IsBitSet(i_bit_index);
	}

	inline size_t BitArray::Size() const
	{
		return num_bits_;
	}

	inline size_t BitArray::GetRequiredMemorySize(size_t i_num_bits)
	{
		const size_t num_buckets = ((i_num_bits & (bit_depth_ - 1)) ? 1 : 0) + i_num_bits / bit_depth_;
		return sizeof(BitArray) + sizeof(size_t) * num_buckets;
	}

} // namespace data
} // namespace engine