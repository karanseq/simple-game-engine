#include "BitArray.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace data {

	inline BitArray& BitArray::operator=(BitArray&& bit_array)
	{
		// check for self assignment
		if (this != &bit_array)
		{
			std::swap(buckets_, bit_array.buckets_);
			num_buckets_ = bit_array.num_buckets_;
			num_bits_ = bit_array.num_bits_;
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

	inline bool BitArray::IsBitSet(size_t bit_index) const
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / bit_depth_;

		return (buckets_[bucket_index] >> bit_index) & static_cast<size_t>(1);
	}

	inline bool BitArray::IsBitClear(size_t bit_index) const
	{
		return !IsBitSet(bit_index);
	}

	inline bool BitArray::Get(size_t bit_index) const
	{
		return IsBitSet(bit_index);
	}

	inline size_t BitArray::Size() const
	{
		return num_bits_;
	}

	inline size_t BitArray::GetRequiredMemorySize(size_t num_bits)
	{
		const size_t num_buckets = ((num_bits & (bit_depth_ - 1)) ? 1 : 0) + num_bits / bit_depth_;
		return sizeof(BitArray) + sizeof(size_t) * num_buckets;
	}

} // namespace data
} // namespace engine