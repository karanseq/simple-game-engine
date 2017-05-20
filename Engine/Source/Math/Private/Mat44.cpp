#include "Math\Mat44.h"

// library includes
#include <math.h>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Math\MathUtil.h"
#include "Math\Vec4D.h"

namespace engine {
namespace math {

const Mat44 Mat44::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f);

const Mat44 Mat44::ZERO(0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f, 
                        0.0f, 0.0f, 0.0f, 0.0f, 
                        0.0f, 0.0f, 0.0f, 0.0f);

Mat44::Mat44()
{
#ifdef BUILD_DEBUG
    f11 = NAN;
#endif
}

Mat44::~Mat44()
{}

Mat44::Mat44(float i_11, float i_12, float i_13, float i_14,
    float i_21, float i_22, float i_23, float i_24,
    float i_31, float i_32, float i_33, float i_34,
    float i_41, float i_42, float i_43, float i_44) : 
    f11(i_11), f12(i_12), f13(i_13), f14(i_14),
    f21(i_21), f22(i_22), f23(i_23), f24(i_24),
    f31(i_31), f32(i_32), f33(i_33), f34(i_34),
    f41(i_41), f42(i_42), f43(i_43), f44(i_44)
{
    ASSERT(!(IsNaN(i_11) || IsNaN(i_12) || IsNaN(i_13) || IsNaN(i_14)));
    ASSERT(!(IsNaN(i_21) || IsNaN(i_22) || IsNaN(i_23) || IsNaN(i_24)));
    ASSERT(!(IsNaN(i_31) || IsNaN(i_32) || IsNaN(i_33) || IsNaN(i_34)));
    ASSERT(!(IsNaN(i_41) || IsNaN(i_42) || IsNaN(i_43) || IsNaN(i_44)));
}

void Mat44::Set(float i_11, float i_12, float i_13, float i_14,
    float i_21, float i_22, float i_23, float i_24,
    float i_31, float i_32, float i_33, float i_34,
    float i_41, float i_42, float i_43, float i_44)
{
    ASSERT(!(IsNaN(i_11) || IsNaN(i_12) || IsNaN(i_13) || IsNaN(i_14)));
    ASSERT(!(IsNaN(i_21) || IsNaN(i_22) || IsNaN(i_23) || IsNaN(i_24)));
    ASSERT(!(IsNaN(i_31) || IsNaN(i_32) || IsNaN(i_33) || IsNaN(i_34)));
    ASSERT(!(IsNaN(i_41) || IsNaN(i_42) || IsNaN(i_43) || IsNaN(i_44)));

    f11 = i_11;
    f12 = i_12;
    f13 = i_13;
    f14 = i_14;

    f21 = i_21;
    f22 = i_22;
    f23 = i_23;
    f24 = i_24;

    f31 = i_31;
    f32 = i_32;
    f33 = i_33;
    f34 = i_34;

    f41 = i_41;
    f42 = i_42;
    f43 = i_43;
    f44 = i_44;
}

Mat44::Mat44(const Mat44& i_copy) :
    f11(i_copy.f11), f12(i_copy.f12), f13(i_copy.f13), f14(i_copy.f14),
    f21(i_copy.f21), f22(i_copy.f22), f23(i_copy.f23), f24(i_copy.f24),
    f31(i_copy.f31), f32(i_copy.f32), f33(i_copy.f33), f34(i_copy.f34),
    f41(i_copy.f41), f42(i_copy.f42), f43(i_copy.f43), f44(i_copy.f44)
{}

Mat44& Mat44::operator=(const Mat44& i_other)
{
    if (this != &i_other)
    {
        f11 = i_other.f11;
        f12 = i_other.f12;
        f13 = i_other.f13;
        f14 = i_other.f14;

        f21 = i_other.f21;
        f22 = i_other.f22;
        f23 = i_other.f23;
        f24 = i_other.f24;

        f31 = i_other.f31;
        f32 = i_other.f32;
        f33 = i_other.f33;
        f34 = i_other.f34;

        f41 = i_other.f41;
        f42 = i_other.f42;
        f43 = i_other.f43;
        f44 = i_other.f44;
    }
    return *this;
}

float Mat44::GetDeterminant() const
{
    float a0 = f11 * f22 - f12 * f21;
    float a1 = f11 * f23 - f13 * f21;
    float a2 = f11 * f24 - f14 * f21;
    float a3 = f12 * f23 - f13 * f22;
    float a4 = f12 * f24 - f14 * f22;
    float a5 = f13 * f24 - f14 * f23;
    float b0 = f31 * f42 - f32 * f41;
    float b1 = f31 * f43 - f33 * f41;
    float b2 = f31 * f44 - f34 * f41;
    float b3 = f32 * f43 - f33 * f42;
    float b4 = f32 * f44 - f34 * f42;
    float b5 = f33 * f44 - f34 * f43;

    return (a0 * b5 + a5 * b0 + a3 * b2 + a2 * b3 - a1 * b4 - a4 * b1);
}

void Mat44::Invert()
{
    float a0 = f11 * f22 - f12 * f21;
    float a1 = f11 * f23 - f13 * f21;
    float a2 = f11 * f24 - f14 * f21;
    float a3 = f12 * f23 - f13 * f22;
    float a4 = f12 * f24 - f14 * f22;
    float a5 = f13 * f24 - f14 * f23;
    float b0 = f31 * f42 - f32 * f41;
    float b1 = f31 * f43 - f33 * f41;
    float b2 = f31 * f44 - f34 * f41;
    float b3 = f32 * f43 - f33 * f42;
    float b4 = f32 * f44 - f34 * f42;
    float b5 = f33 * f44 - f34 * f43;

    float det = a0 * b5 + a5 * b0 + a3 * b2 + a2 * b3 - a1 * b4 - a4 * b1;

    if (FuzzyEquals(det, 0.0f))
    {
        LOG_ERROR("This matrix is not invertible!");
        return;
    }

    Mat44 adjoin;
    adjoin.f11 =  f22 * b5 - f23 * b4 + f24 * b3;
    adjoin.f12 = -f12 * b5 + f13 * b4 - f14 * b3;
    adjoin.f13 =  f42 * a5 - f43 * a4 + f44 * a3;
    adjoin.f14 = -f32 * a5 + f33 * a4 - f34 * a3;

    adjoin.f21 = -f21 * b5 + f23 * b2 - f24 * b1;
    adjoin.f22 =  f11 * b5 - f13 * b2 + f14 * b1;
    adjoin.f23 = -f41 * a5 + f43 * a2 - f44 * a1;
    adjoin.f24 =  f31 * a5 - f33 * a2 + f34 * a1;

    adjoin.f31 =  f21 * b4 - f22 * b2 + f24 * b0;
    adjoin.f32 = -f11 * b4 + f12 * b2 - f14 * b0;
    adjoin.f33 =  f41 * a4 - f42 * a2 + f44 * a0;
    adjoin.f34 = -f31 * a4 + f32 * a2 - f34 * a0;

    adjoin.f41 = -f21 * b3 + f22 * b1 - f23 * b0;
    adjoin.f42 =  f11 * b3 - f12 * b1 + f13 * b0;
    adjoin.f43 = -f41 * a3 + f42 * a1 - f43 * a0;
    adjoin.f44 =  f31 * a3 - f32 * a1 + f33 * a0;

    // only divide by determinant if determinant is not 1
    if (!FuzzyEquals(det, 1.0f))
    {
        float inverse_det = 1.0f / det;

        adjoin.f11 *= inverse_det;
        adjoin.f12 *= inverse_det;
        adjoin.f13 *= inverse_det;
        adjoin.f14 *= inverse_det;

        adjoin.f21 *= inverse_det;
        adjoin.f22 *= inverse_det;
        adjoin.f23 *= inverse_det;
        adjoin.f24 *= inverse_det;

        adjoin.f31 *= inverse_det;
        adjoin.f32 *= inverse_det;
        adjoin.f33 *= inverse_det;
        adjoin.f34 *= inverse_det;

        adjoin.f41 *= inverse_det;
        adjoin.f42 *= inverse_det;
        adjoin.f43 *= inverse_det;
        adjoin.f44 *= inverse_det;
    }

    *this = adjoin;
}

Mat44 Mat44::GetInverse() const
{
    Mat44 inverse(*this);
    inverse.Invert();
    return inverse;
}

void Mat44::Transpose()
{
    float temp = f21;
    f21 = f12;
    f12 = temp;

    temp = f31;
    f31 = f13;
    f13 = temp;

    temp = f41;
    f41 = f14;
    f14 = temp;

    temp = f32;
    f32 = f23;
    f23 = temp;

    temp = f42;
    f42 = f24;
    f24 = temp;

    temp = f43;
    f43 = f34;
    f34 = temp;
}

Mat44 Mat44::GetTranspose() const
{
    Mat44 transpose(*this);
    transpose.Transpose();
    return transpose;
}

//Mat44 Mat44::operator*(float i_scalar)
//{
//    // validate input
//    ASSERT(!IsNaN(i_scalar));
//
//    Mat44 result(f11 * i_scalar, f12 * i_scalar, f13 * i_scalar, f14 * i_scalar,
//                 f21 * i_scalar, f22 * i_scalar, f23 * i_scalar, f24 * i_scalar,
//                 f31 * i_scalar, f32 * i_scalar, f33 * i_scalar, f34 * i_scalar,
//                 f41 * i_scalar, f42 * i_scalar, f43 * i_scalar, f44 * i_scalar);
//
//    return result;
//}

bool Mat44::operator==(const Mat44& i_other) const
{
    if (this == &i_other)
    {
        return true;
    }

    return !(!FuzzyEquals(f11, i_other.f11) || !FuzzyEquals(f12, i_other.f12) || !FuzzyEquals(f13, i_other.f13) || !FuzzyEquals(f14, i_other.f14) ||
             !FuzzyEquals(f21, i_other.f21) || !FuzzyEquals(f22, i_other.f22) || !FuzzyEquals(f23, i_other.f23) || !FuzzyEquals(f24, i_other.f24) ||
             !FuzzyEquals(f31, i_other.f31) || !FuzzyEquals(f32, i_other.f32) || !FuzzyEquals(f33, i_other.f33) || !FuzzyEquals(f34, i_other.f34) ||
             !FuzzyEquals(f41, i_other.f41) || !FuzzyEquals(f42, i_other.f42) || !FuzzyEquals(f43, i_other.f43) || !FuzzyEquals(f44, i_other.f44));
}

// this * i_right_mat
Mat44 Mat44::Multiply(const Mat44& i_right_mat) const
{
    Mat44 result;

    result.f11 = f11 * i_right_mat.f11 + f12 * i_right_mat.f21 + f13 * i_right_mat.f31 + f14 * i_right_mat.f41;
    result.f12 = f11 * i_right_mat.f12 + f12 * i_right_mat.f22 + f13 * i_right_mat.f32 + f14 * i_right_mat.f42;
    result.f13 = f11 * i_right_mat.f13 + f12 * i_right_mat.f23 + f13 * i_right_mat.f33 + f14 * i_right_mat.f43;
    result.f14 = f11 * i_right_mat.f14 + f12 * i_right_mat.f24 + f13 * i_right_mat.f34 + f14 * i_right_mat.f44;

    result.f21 = f21 * i_right_mat.f11 + f22 * i_right_mat.f21 + f23 * i_right_mat.f31 + f24 * i_right_mat.f41;
    result.f22 = f21 * i_right_mat.f12 + f22 * i_right_mat.f22 + f23 * i_right_mat.f32 + f24 * i_right_mat.f42;
    result.f23 = f21 * i_right_mat.f13 + f22 * i_right_mat.f23 + f23 * i_right_mat.f33 + f24 * i_right_mat.f43;
    result.f24 = f21 * i_right_mat.f14 + f22 * i_right_mat.f24 + f23 * i_right_mat.f34 + f24 * i_right_mat.f44;

    result.f31 = f31 * i_right_mat.f11 + f32 * i_right_mat.f21 + f33 * i_right_mat.f31 + f34 * i_right_mat.f41;
    result.f32 = f31 * i_right_mat.f12 + f32 * i_right_mat.f22 + f33 * i_right_mat.f32 + f34 * i_right_mat.f42;
    result.f33 = f31 * i_right_mat.f13 + f32 * i_right_mat.f23 + f33 * i_right_mat.f33 + f34 * i_right_mat.f43;
    result.f34 = f31 * i_right_mat.f14 + f32 * i_right_mat.f24 + f33 * i_right_mat.f34 + f34 * i_right_mat.f44;

    result.f41 = f41 * i_right_mat.f11 + f42 * i_right_mat.f21 + f43 * i_right_mat.f31 + f44 * i_right_mat.f41;
    result.f42 = f41 * i_right_mat.f12 + f42 * i_right_mat.f22 + f43 * i_right_mat.f32 + f44 * i_right_mat.f42;
    result.f43 = f41 * i_right_mat.f13 + f42 * i_right_mat.f23 + f43 * i_right_mat.f33 + f44 * i_right_mat.f43;
    result.f44 = f41 * i_right_mat.f14 + f42 * i_right_mat.f24 + f43 * i_right_mat.f34 + f44 * i_right_mat.f44;

    return result;
}

// i_left_vec4 * this
Vec4D Mat44::MultiplyLeft(const Vec4D& i_left_vec4) const
{
    Vec4D result;

    result.x(i_left_vec4.x() * f11 + i_left_vec4.y() * f21 + i_left_vec4.z() * f31 + i_left_vec4.w() * f41);
    result.y(i_left_vec4.x() * f12 + i_left_vec4.y() * f22 + i_left_vec4.z() * f32 + i_left_vec4.w() * f42);
    result.z(i_left_vec4.x() * f13 + i_left_vec4.y() * f23 + i_left_vec4.z() * f33 + i_left_vec4.w() * f43);
    result.w(i_left_vec4.x() * f14 + i_left_vec4.y() * f24 + i_left_vec4.z() * f34 + i_left_vec4.w() * f44);

    return result;
}

// this * i_right_vec4
Vec4D Mat44::MultiplyRight(const Vec4D& i_right_vec4) const
{
    Vec4D result;

    result.x(f11 * i_right_vec4.x() + f12 * i_right_vec4.y() + f13 * i_right_vec4.z() + f14 * i_right_vec4.w());
    result.y(f21 * i_right_vec4.x() + f22 * i_right_vec4.y() + f23 * i_right_vec4.z() + f24 * i_right_vec4.w());
    result.z(f31 * i_right_vec4.x() + f32 * i_right_vec4.y() + f33 * i_right_vec4.z() + f34 * i_right_vec4.w());
    result.w(f41 * i_right_vec4.x() + f42 * i_right_vec4.y() + f43 * i_right_vec4.z() + f44 * i_right_vec4.w());

    return result;
}

#ifdef BUILD_DEBUG
void Mat44::Print() const
{
    LOG("\n%f, \t%f, \t%f, \t%f\n%f, \t%f, \t%f, \t%f\n%f, \t%f, \t%f, \t%f\n%f, \t%f, \t%f, \t%f",
        f11, f12, f13, f14,
        f21, f22, f23, f24,
        f31, f32, f33, f34,
        f41, f42, f43, f44);
}
#endif

Mat44 Mat44::GetTranslation(const Vec3D& i_translation)
{
    return Mat44(1.0, 0.0f, 0.0f, i_translation.x(),
                 0.0f, 1.0f, 0.0f, i_translation.y(),
                 0.0f, 0.0f, 1.0f, i_translation.z(),
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat44 Mat44::GetRotationX(const float i_radians)
{
    ASSERT(!IsNaN(i_radians));
    return Mat44(1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, cosf(i_radians), -sinf(i_radians), 0.0f,
                 0.0f, sinf(i_radians), cosf(i_radians), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat44 Mat44::GetRotationY(const float i_radians)
{
    ASSERT(!IsNaN(i_radians));
    return Mat44(cosf(i_radians), 0, sinf(i_radians), 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 -sinf(i_radians), 0.0f, cosf(i_radians), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat44 Mat44::GetRotationZ(const float i_radians)
{
    ASSERT(!IsNaN(i_radians));
    return Mat44(cosf(i_radians), -sinf(i_radians), 0.0f, 0.0f,
                 sinf(i_radians), cosf(i_radians), 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat44 Mat44::GetScale(const float i_scale)
{
    ASSERT(!IsNaN(i_scale));
    return Mat44(i_scale, 0.0f, 0.0f, 0.0f,
                 0.0f, i_scale, 0.0f, 0.0f,
                 0.0f, 0.0f, i_scale, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

} // namespace math
} // namespace engine