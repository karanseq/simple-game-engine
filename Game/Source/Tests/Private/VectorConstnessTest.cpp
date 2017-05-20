// library includes
#include <stdio.h>

// engine includes
#include "Logger\Logger.h"
#include "Math\Vec2D.h"
#include "Math\Vec3D.h"

void VecToString(const engine::math::Vec2D& i_vec, char* o_buf)
{
    ASSERT(o_buf);
    sprintf_s(o_buf, 128, "%f, %f", i_vec.x(), i_vec.y());
}

void VecToString(const engine::math::Vec3D& i_vec, char* o_buf)
{
    ASSERT(o_buf);
    sprintf_s(o_buf, 128, "%f, %f, %f", i_vec.x(), i_vec.y(), i_vec.z());
}

void TestVectorConstness()
{
    LOG("-------------------- Running Vec2D, Vec3D Test --------------------");

    char buf[128] = { 0 };

    const engine::math::Vec2D       v2_a(2.0f, 4.0f);
    VecToString(v2_a, buf);
    LOG("A : %s", buf);
    const engine::math::Vec2D       v2_b(3.0f, 5.0f);
    VecToString(v2_b, buf);
    LOG("B : %s", buf);

    const engine::math::Vec2D       v2_c = v2_a + v2_b;
    VecToString(v2_c, buf);
    LOG("C = A + B : %s", buf);

    engine::math::Vec2D             v2_d = -v2_c;
    VecToString(v2_d, buf);
    LOG("D = -C : %s", buf);

    const engine::math::Vec3D       v3_a(2.0f, 4.0f, 6.0f);
    VecToString(v3_a, buf);
    LOG("A : %s", buf);
    const engine::math::Vec3D       v3_b(3.0f, 5.0f, 7.0f);
    VecToString(v3_b, buf);
    LOG("B : %s", buf);

    const engine::math::Vec3D       v3_c = v3_a + v3_b;
    VecToString(v3_c, buf);
    LOG("C = A + B : %s", buf);

    engine::math::Vec3D             v3_d = -v3_c;
    VecToString(v3_d, buf);
    LOG("D = -C : %s", buf);

    LOG("-------------------- Finished Vec2D, Vec3D Test --------------------");
}
