#include "StringPool.h"

namespace engine {
namespace data {

inline StringPool* StringPool::Get()
{
	return StringPool::instance_;
}

inline bool StringPool::Contains(const char* i_string) const
{
	std::lock_guard<std::mutex> lock(pool_mutex_);
	const uint8_t* pointer = reinterpret_cast<const uint8_t*>(i_string);
	return (pointer > pool_ && pointer < pool_ + pool_size_);
}

} // namespace data
} // namespace engine
