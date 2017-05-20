#if defined(ENABLE_PROFILING)

#include "Util\Profiler.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Logger\Logger.h"
#include "Time\TimerUtil.h"

namespace engine {
namespace util {

Profiler* Profiler::instance_ = nullptr;

Profiler::Profiler()
{
    accumulators_.reserve(20);
}

Profiler::~Profiler()
{
    DumpStatistics();
    accumulators_.clear();
}

Profiler* Profiler::Create()
{
    if (Profiler::instance_ == nullptr)
    {
        Profiler::instance_ = new Profiler();
    }
    return Profiler::instance_;
}

void Profiler::Destroy()
{
    SAFE_DELETE(Profiler::instance_);
}

void Profiler::RegisterAccumulator(const char* i_name, Accumulator* i_accumulator)
{
    accumulators_.insert(std::make_pair(i_name, i_accumulator));
}

void Profiler::DumpStatistics()
{
    LOG("---------- %s ----------", __FUNCTION__);
    LOG("Dumping profiler statistics:");
    for (const auto& i : accumulators_)
    {
        double timer_frequency = engine::time::TimerUtil::GetFrequency();
        LOG("%s - min:%1.8fms average:%1.8fms max:%1.8fms called:%d times", i.first, i.second->min_ / timer_frequency, i.second->Average() / timer_frequency, i.second->max_ / timer_frequency, i.second->count_);
    }
    LOG("---------- END ----------");
}

Accumulator::Accumulator(const char* i_name) : sum_(0.0),
    count_(0),
    min_(std::numeric_limits<double>::max()),
    max_(std::numeric_limits<double>::min())
{
    Profiler::Get()->RegisterAccumulator(i_name, this);
}

ScopedTimer::ScopedTimer(Accumulator* i_accumulator) : start_(engine::time::TimerUtil::CalculateTick()),
    accumulator_(i_accumulator)
{}

ScopedTimer::~ScopedTimer()
{
    *accumulator_ += engine::time::TimerUtil::CalculateTick() - start_;
}

} // namespace util
} // namespace engine

#endif // ENABLED_PROFILING
