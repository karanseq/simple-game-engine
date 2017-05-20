#include "Logger\Logger.h"
#include "Math\MathUtil.h"

void TestFloatValidity()
{
    LOG("-------------------- Running Float Validity Test --------------------");

    float           some_float = -1.0f;
    LOG("%s a NaN!", engine::math::IsNaN(some_float) ? "Is" : "Isn't");

    const float     nan = nanf("foo");
    LOG("%s a NaN!", engine::math::IsNaN(nan) ? "Is" : "Isn't");

    LOG("-------------------- Finished Float Validity Test --------------------");
}