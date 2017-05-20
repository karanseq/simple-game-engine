#include "HashedString.h"

// library includes
#include <string.h>

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace data {

inline HashedString& HashedString::operator=(const HashedString& i_copy)
{
    if (this != &i_copy)
    {
        hash_ = i_copy.hash_;
    }
    return *this;
}

inline bool HashedString::operator==(const HashedString& i_other) const
{
    return (hash_ == i_other.hash_);
}

inline bool HashedString::operator==(const char* i_other) const
{
    return (hash_ == HashedString::Hash(i_other));
}

inline bool HashedString::operator!=(const HashedString& i_other) const
{
    return (hash_ != i_other.hash_);
}

inline bool HashedString::operator!=(const char* i_other) const
{
    return (hash_ != HashedString::Hash(i_other));
}

inline bool HashedString::operator<(const HashedString& i_other) const
{
    return (hash_ < i_other.hash_);
}

inline bool HashedString::operator>(const HashedString& i_other) const
{
    return (hash_ > i_other.hash_);
}

inline unsigned int HashedString::GetHash() const
{
    return hash_;
}

inline unsigned int HashedString::Hash(const char* i_string)
{
    ASSERT(i_string);
    return Hash(reinterpret_cast<const void*>(i_string), static_cast<unsigned int>(strlen(i_string)));
}

inline unsigned int HashedString::Hash(const PooledString& i_string)
{
    ASSERT(i_string);
    return Hash(reinterpret_cast<const void*>(i_string.GetString()), static_cast<unsigned int>(strlen(i_string.GetString())));
}

} // namespace data
} // namespace engine
