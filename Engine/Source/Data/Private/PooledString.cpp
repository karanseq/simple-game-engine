#include "Data\PooledString.h"

namespace engine {
namespace data {

PooledString::PooledString() : string_(StringPool::Get()->Add(""))
{}

PooledString::PooledString(const char* i_string) : string_(StringPool::Get()->Add(i_string))
{}

PooledString::~PooledString()
{}

PooledString::PooledString(const PooledString& i_copy) : string_(i_copy.string_)
{}

} // namespace data
} // namespace engine
