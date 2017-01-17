#ifndef ENGINE_BIT_ARRAY_H_
#define ENGINE_BIT_ARRAY_H_

namespace engine {
namespace data {

	/*
		BitArray
		- Creates and maintains an array of bits
		- Bits can be set, cleared or toggled both individually and altogether
		- Needs to be provided raw memory to create an instance of itself
	*/

	class BitArray
	{
	private:
		BitArray(size_t num_bits, void* memory, bool start_set = false);

		// disable copy constructor & copy assignment operator
		BitArray(const BitArray& copy);
		inline BitArray& operator=(const BitArray& bit_array);

	public:
		static BitArray* Create(size_t num_bits, void* memory, bool start_set = false);
		~BitArray();
		
		BitArray(BitArray&& copy);

		inline BitArray& operator=(BitArray&& bit_array);

		inline void ClearAll();
		inline void SetAll();
		inline void ToggleAll();

		bool AreAllClear() const;
		bool AreAllSet() const;

		inline bool IsBitSet(size_t bit_index) const;
		inline bool IsBitClear(size_t bit_index) const;

		void SetBit(size_t bit_index);
		void ClearBit(size_t bit_index);
		void ToggleBit(size_t bit_index);

		bool GetFirstSetBit(size_t &bit_index) const;
		bool GetFirstClearBit(size_t &bit_index) const;

		inline bool Get(size_t bit_index) const;
		inline size_t Size() const;

		static inline size_t GetRequiredMemorySize(size_t num_bits);

	private:
		size_t*											buckets_;
		size_t											num_buckets_;
		size_t											num_bits_;
		static const size_t								bit_depth_;

	}; // class BitArray

} // namespace data
} // namespace engine

#include "BitArray-inl.h"

#endif // ENGINE_BIT_ARRAY_H_