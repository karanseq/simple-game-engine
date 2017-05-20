#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

// library includes
#include <math.h>

#define M_PI                    3.14159265358979323846f  /* pi */
#define MIN_EPSILON             0.000000001f
#define MAX_EPSILON             0.0001f

namespace engine {
namespace math {

// forward declarations
struct AABB;
class Mat44;
class Transform;
class Vec2D;
class Vec3D;
class Vec4D;

namespace optimized {
class Mat44;
class Vec3D;
}

inline float RadiansToDegrees(float i_radians)
{
    return (i_radians * 180.0f / M_PI);
}

inline float DegreesToRadians(float i_degrees)
{
    return (i_degrees * M_PI / 180.0f);
}

inline bool IsNaN(float i_number)
{
    volatile float temp = i_number;
    return (temp != i_number);
}

inline bool FuzzyEquals(float i_lhs, float i_rhs, float i_epsilon = MAX_EPSILON)
{
    return fabs(i_rhs - i_lhs) < i_epsilon;
}

inline bool IsZero(float i_number)
{
    return FuzzyEquals(i_number, MIN_EPSILON);
}

inline float GetMinOfFour(float i_first, float i_second, float i_third, float i_fourth)
{
    float first_two = i_first < i_second ? i_first : i_second;
    float last_two = i_third < i_fourth ? i_third : i_fourth;
    return first_two < last_two ? first_two : last_two;
}

inline float GetMaxOfFour(float i_first, float i_second, float i_third, float i_fourth)
{
    float first_two = i_first > i_second ? i_first : i_second;
    float last_two = i_third > i_fourth ? i_third : i_fourth;
    return first_two > last_two ? first_two : last_two;
}

// dot products
float DotProduct(const Vec2D& i_v1, const Vec2D& i_v2);
float DotProduct(const Vec3D& i_v1, const Vec3D& i_v2);
float DotProduct(const engine::math::optimized::Vec3D& i_v1, const engine::math::optimized::Vec3D& i_v2);

// cross product
Vec3D CrossProduct(const Vec3D& i_v1, const Vec3D& i_v2);
engine::math::optimized::Vec3D CrossProduct(const engine::math::optimized::Vec3D& i_v1, const engine::math::optimized::Vec3D& i_v2);

// transforms
// the transform matrix as a result of this function must be left multiplied
// with the vector/point that will be transformed
void GetObjectToWorldTransform(const engine::math::Transform& i_transform, engine::math::Mat44& o_trans_mat);
void GetObjectToWorldTransform(const engine::math::Transform& i_transform, engine::math::optimized::Mat44& o_trans_mat);

} // namespace math
} // namespace engine

#endif // MATH_UTIL_H_

