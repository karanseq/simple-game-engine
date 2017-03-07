#ifndef REF_COUNTER_H_
#define REF_COUNTER_H_

namespace engine {
namespace memory {

struct RefCounter
{
public:
    explicit RefCounter(long i_strong_count = 0, long i_weak_count = 0) : strong_count(i_strong_count),
        weak_count(i_weak_count)
    {}
    long strong_count;
    long weak_count;
}; // struct RefCounter

} // namespace memory
} // namespace engine

#endif // REF_COUNTER_H_
