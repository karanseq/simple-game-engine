#ifdef BUILD_DEBUG

#include "AllocationCounter.h"

namespace engine {
namespace memory {

inline bool AllocationCount::operator<(const AllocationCount& i_other) const
{
    return allocation_ < i_other.allocation_;
}

inline bool AllocationCount::operator==(const AllocationCount& i_other) const
{
    return allocation_ == i_other.allocation_;
}

inline AllocationCount& AllocationCount::operator++()
{
    ++count_;
    return *this;
}

inline AllocationCount& AllocationCount::operator++(int)
{
    ++count_;
    return *this;
}

inline AllocationCount& AllocationCount::operator+=(size_t i_count)
{
    count_ += i_count;
    return *this;
}

inline AllocationCounter* AllocationCounter::Get()
{
    return AllocationCounter::instance_;
}

} // namespace memory
} // namespace engine

#endif // BUILD_DEBUG
