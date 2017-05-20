// library includes
#include <stdio.h>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Math\MathUtil.h"
#include "Math\Mat44.h"
#include "Math\Mat44-SSE.h"
#include "Math\Vec3D.h"
#include "Math\Vec4D.h"
#include "Math\Vec3D-SSE.h"
#include "Math\Vec4D-SSE.h"

void TestFastMath()
{
    LOG("-------------------- Running Fast Math Test --------------------");

    using namespace engine::math;

    //~==================================================================
    // Vec3D
    optimized::Vec3D vec01(1.0f, 2.0f, 3.0f);
    vec01 *= 2.0f;

    float length_squared_01 = vec01.LengthSquared();
    ASSERT(engine::math::FuzzyEquals(length_squared_01, 56.0f));

    optimized::Vec3D vec02(2.0f, 4.0f, 6.0f);
    ASSERT(vec01 == vec02);

    optimized::Vec3D vec03(-1.0f, 11.0f, -22.0f);

    Vec3D slow01(2.0f, 4.0f, 6.0f);
    Vec3D slow02(-1.0f, 11.0f, -22.0f);

    ASSERT(FuzzyEquals(DotProduct(vec02, vec03), DotProduct(slow01, slow02)));

    optimized::Vec3D fast_cross = CrossProduct(vec02, vec03);
    Vec3D slow_cross = CrossProduct(slow01, slow02);

    ASSERT(fast_cross == optimized::Vec3D(slow_cross.x(), slow_cross.y(), slow_cross.z()));

    //~==================================================================
    // Vec4D
    optimized::Vec4D vec11(1.0f, 2.0f, 3.0f, 4.0f);
    vec11 *= 2.0f;

    float length_squared_11 = vec11.LengthSquared();
    ASSERT(engine::math::FuzzyEquals(length_squared_11, 120.0f));

    optimized::Vec4D vec12(2.0f, 4.0f, 6.0f, 8.0f);
    ASSERT(vec11 == vec12);

    //~==================================================================
    // Mat44
    optimized::Mat44 mat_01(1.0f, 2.0f, 3.0f, 4.0f,
        1.0f, 2.0f, 3.0f, 4.0f,
        1.0f, 2.0f, 3.0f, 4.0f,
        1.0f, 2.0f, 3.0f, 4.0f);

    optimized::Mat44 mat_02(11.0f, 12.0f, 13.0f, 14.0f,
        21.0f, 22.0f, 23.0f, 24.0f,
        31.0f, 32.0f, 33.0f, 34.0f,
        41.0f, 42.0f, 43.0f, 44.0f);

    //ASSERT(mat_01.GetTranspose() == mat_02);

    optimized::Mat44 mat03 = mat_01.Multiply(mat_02);

    LOG("-------------------- Finished Fast Math Test --------------------");
}

