#ifndef PROFILER_H_
#define PROFILER_H_

#if defined(ENABLE_PROFILING)

// library includes
#include <limits>
#include <unordered_map>
#include <stdint.h>

#define CONCAT_HELPER(left, right) left##right
#define CONCAT(left, right) CONCAT_HELPER(left, right)

#define PROFILE_UNSCOPED(name)                         \
    static engine::util::Accumulator CONCAT(__Accumulator, __LINE__)(name); engine::util::ScopedTimer CONCAT(__Timer, __LINE__)(&CONCAT(__Accumulator, __LINE__));

#define PROFILE_SCOPE_BEGIN(name) {                    \
    static engine::util::Accumulator CONCAT(__Accumulator, __LINE__)(name); engine::util::ScopedTimer CONCAT(__Timer, __LINE__)(&CONCAT(__Accumulator, __LINE__));
#define PROFILE_SCOPE_END         }

#else

#define PROFILE_UNSCOPED(str)           //__noop
#define PROFILE_SCOPE_BEGIN(str)        //__noop
#define PROFILE_SCOPE_END               //__noop

#endif

namespace engine {
namespace util {

#if defined(ENABLE_PROFILING)

// forward declaration
class Accumulator;

class Profiler
{
public:
    static Profiler* Create();
    static void Destroy();
    static inline Profiler* Get() { return Profiler::instance_; }

    void RegisterAccumulator(const char* i_name, Accumulator* i_accumulator);
    void DumpStatistics();

private:
    Profiler();
    ~Profiler();
    static Profiler* instance_;

    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    std::unordered_map<const char*, Accumulator*>       accumulators_;

}; // class Profiler

class Accumulator
{
public:
    double                  sum_;
    uint64_t                count_;
    double                  min_;
    double                  max_;

    Accumulator(const char* i_name);

    inline void operator+=(double i_time)
    {
        sum_ += i_time;
        ++count_;

        min_ = i_time < min_ ? i_time : min_;
        max_ = i_time > max_ ? i_time : max_;
    }

    inline double Average() const
    {
        return sum_ / count_;
    }

}; // class Accumulator

class ScopedTimer
{
public:
    ScopedTimer(Accumulator* i_accumulator);
    ~ScopedTimer();

private:
    double                  start_;
    Accumulator*            accumulator_;

}; // class ScopedTimer

#endif // ENABLE_PROFILING

} // namespace util
} // namespace engine

#endif // PROFILER_H_

