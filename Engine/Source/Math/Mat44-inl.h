#include "Mat44.h"

namespace engine {
namespace math {

inline bool Mat44::IsValid() const
{
    return !IsNaN(f11);
}

inline Mat44::operator bool() const
{
    return IsValid();
}

inline Mat44 operator*(const Mat44& i_left, const Mat44& i_right)
{
    return i_left.Multiply(i_right);
}

// V = M * V
inline Vec4D operator*(const Mat44& i_left, const Vec4D& i_right)
{
    return i_left.MultiplyRight(i_right);
}

// V = V * M
inline Vec4D operator*(const Vec4D& i_left, const Mat44& i_right)
{
    return i_right.MultiplyLeft(i_left);
}

} // namespace math
} // namespace engine
