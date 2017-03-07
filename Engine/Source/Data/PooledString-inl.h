#include "PooledString.h"

namespace engine {
namespace data {

inline PooledString& PooledString::operator=(const PooledString& i_copy)
{
    // check for self assignment
    if (this != &i_copy)
    {
        string_ = i_copy.string_;
    }
    return *this;
}

inline bool PooledString::operator==(const PooledString& i_other) const
{
    return string_ == i_other.string_;
}

inline bool PooledString::operator!=(const PooledString& i_other) const
{
    return string_ != i_other.string_;
}

inline PooledString::operator bool() const
{
    return string_ != nullptr;
}

inline const char* PooledString::GetString() const
{
    return string_;
}

inline const size_t PooledString::GetLength() const
{
    return *(reinterpret_cast<const size_t*>(string_ - sizeof(size_t))) - 1;
}

} // namespace data
} // namesapce engine
