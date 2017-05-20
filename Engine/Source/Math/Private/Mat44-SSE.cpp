#include "Math\Mat44-SSE.h"

// library includes
#include <math.h>
#include <stdint.h>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Math\MathUtil.h"
#include "Math\Vec4D-SSE.h"
#include "Math\Vec3D-SSE.h"

namespace engine {
namespace math {
namespace optimized {

const Mat44 Mat44::IDENTITY(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f),
    _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f),
    _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f),
    _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));

const Mat44 Mat44::ZERO(_mm_set1_ps(0.0f),
    _mm_set1_ps(0.0f),
    _mm_set1_ps(0.0f),
    _mm_set1_ps(0.0f));

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
    f41(i_41), f42(i_42), f43(i_43), f44(i_44),
    row1(_mm_setr_ps(i_11, i_12, i_13, i_14)),
    row2(_mm_setr_ps(i_21, i_22, i_23, i_24)),
    row3(_mm_setr_ps(i_31, i_32, i_33, i_34)),
    row4(_mm_setr_ps(i_41, i_42, i_43, i_44))
{
    ASSERT(!(IsNaN(i_11) || IsNaN(i_12) || IsNaN(i_13) || IsNaN(i_14)));
    ASSERT(!(IsNaN(i_21) || IsNaN(i_22) || IsNaN(i_23) || IsNaN(i_24)));
    ASSERT(!(IsNaN(i_31) || IsNaN(i_32) || IsNaN(i_33) || IsNaN(i_34)));
    ASSERT(!(IsNaN(i_41) || IsNaN(i_42) || IsNaN(i_43) || IsNaN(i_44)));
}

Mat44::Mat44(const __m128& i_row1, const __m128& i_row2, const __m128& i_row3, const __m128& i_row4) : row1(i_row1),
    row2(i_row2),
    row3(i_row3),
    row4(i_row4)
{}

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

    row1 = _mm_setr_ps(i_11, i_12, i_13, i_14);
    row2 = _mm_setr_ps(i_21, i_22, i_23, i_24);
    row3 = _mm_setr_ps(i_31, i_32, i_33, i_34);
    row4 = _mm_setr_ps(i_41, i_42, i_43, i_44);
}

void Mat44::Set(const __m128& i_row1, const __m128& i_row2, const __m128& i_row3, const __m128& i_row4)
{
    row1 = i_row1;
    row2 = i_row2;
    row3 = i_row3;
    row4 = i_row4;
}

Mat44::Mat44(const Mat44& i_copy) :
    f11(i_copy.f11), f12(i_copy.f12), f13(i_copy.f13), f14(i_copy.f14),
    f21(i_copy.f21), f22(i_copy.f22), f23(i_copy.f23), f24(i_copy.f24),
    f31(i_copy.f31), f32(i_copy.f32), f33(i_copy.f33), f34(i_copy.f34),
    f41(i_copy.f41), f42(i_copy.f42), f43(i_copy.f43), f44(i_copy.f44),
    row1(i_copy.row1), row2(i_copy.row2), row3(i_copy.row3), row4(i_copy.row4)
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

        row1 = i_other.row1;
        row2 = i_other.row2;
        row3 = i_other.row3;
        row4 = i_other.row4;
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

    return 0.0f;
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
    adjoin.f11 = f22 * b5 - f23 * b4 + f24 * b3;
    adjoin.f12 = -f12 * b5 + f13 * b4 - f14 * b3;
    adjoin.f13 = f42 * a5 - f43 * a4 + f44 * a3;
    adjoin.f14 = -f32 * a5 + f33 * a4 - f34 * a3;

    adjoin.f21 = -f21 * b5 + f23 * b2 - f24 * b1;
    adjoin.f22 = f11 * b5 - f13 * b2 + f14 * b1;
    adjoin.f23 = -f41 * a5 + f43 * a2 - f44 * a1;
    adjoin.f24 = f31 * a5 - f33 * a2 + f34 * a1;

    adjoin.f31 = f21 * b4 - f22 * b2 + f24 * b0;
    adjoin.f32 = -f11 * b4 + f12 * b2 - f14 * b0;
    adjoin.f33 = f41 * a4 - f42 * a2 + f44 * a0;
    adjoin.f34 = -f31 * a4 + f32 * a2 - f34 * a0;

    adjoin.f41 = -f21 * b3 + f22 * b1 - f23 * b0;
    adjoin.f42 = f11 * b3 - f12 * b1 + f13 * b0;
    adjoin.f43 = -f41 * a3 + f42 * a1 - f43 * a0;
    adjoin.f44 = f31 * a3 - f32 * a1 + f33 * a0;

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
    _MM_TRANSPOSE4_PS(row1, row2, row3, row4);
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

    return !(_mm_movemask_ps(_mm_cmpneq_ps(row1, i_other.row1)) != 0 ||
        _mm_movemask_ps(_mm_cmpneq_ps(row2, i_other.row2)) != 0 ||
        _mm_movemask_ps(_mm_cmpneq_ps(row3, i_other.row3)) != 0 ||
        _mm_movemask_ps(_mm_cmpneq_ps(row4, i_other.row4)) != 0);
}

// this * i_right_mat
Mat44 Mat44::Multiply(const Mat44& i_right_mat) const
{
    Mat44 result;

    __m128 col1 = _mm_shuffle_ps(_mm_shuffle_ps(i_right_mat.row1, i_right_mat.row2, _MM_SHUFFLE(0, 0, 0, 0)),
        _mm_shuffle_ps(i_right_mat.row3, i_right_mat.row4, _MM_SHUFFLE(0, 0, 0, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    __m128 col2 = _mm_shuffle_ps(_mm_shuffle_ps(i_right_mat.row1, i_right_mat.row2, _MM_SHUFFLE(0, 1, 0, 1)),
        _mm_shuffle_ps(i_right_mat.row3, i_right_mat.row4, _MM_SHUFFLE(1, 0, 1, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    __m128 col3 = _mm_shuffle_ps(_mm_shuffle_ps(i_right_mat.row1, i_right_mat.row2, _MM_SHUFFLE(0, 2, 0, 2)),
        _mm_shuffle_ps(i_right_mat.row3, i_right_mat.row4, _MM_SHUFFLE(2, 0, 2, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    __m128 col4 = _mm_shuffle_ps(_mm_shuffle_ps(i_right_mat.row1, i_right_mat.row2, _MM_SHUFFLE(0, 3, 0, 3)),
        _mm_shuffle_ps(i_right_mat.row3, i_right_mat.row4, _MM_SHUFFLE(3, 0, 3, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    result.row1 = _mm_setr_ps(_mm_cvtss_f32(_mm_dp_ps(row1, col1, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row1, col2, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row1, col3, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row1, col4, 0xFF)));

    result.row2 = _mm_setr_ps(_mm_cvtss_f32(_mm_dp_ps(row2, col1, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row2, col2, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row2, col3, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row2, col4, 0xFF)));

    result.row3 = _mm_setr_ps(_mm_cvtss_f32(_mm_dp_ps(row3, col1, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row3, col2, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row3, col3, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row3, col4, 0xFF)));

    result.row4 = _mm_setr_ps(_mm_cvtss_f32(_mm_dp_ps(row4, col1, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row4, col2, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row4, col3, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row4, col4, 0xFF)));

    return result;
}

// i_left_vec4 * this
Vec4D Mat44::MultiplyLeft(const Vec4D& i_left_vec4) const
{
    Vec4D result;

    __m128 col1 = _mm_shuffle_ps(_mm_shuffle_ps(row1, row2, _MM_SHUFFLE(0, 0, 0, 0)),
        _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(0, 0, 0, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    __m128 col2 = _mm_shuffle_ps(_mm_shuffle_ps(row1, row2, _MM_SHUFFLE(0, 1, 0, 1)),
        _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(1, 0, 1, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    __m128 col3 = _mm_shuffle_ps(_mm_shuffle_ps(row1, row2, _MM_SHUFFLE(0, 2, 0, 2)),
        _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(2, 0, 2, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    __m128 col4 = _mm_shuffle_ps(_mm_shuffle_ps(row1, row2, _MM_SHUFFLE(0, 3, 0, 3)),
        _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(3, 0, 3, 0)),
        _MM_SHUFFLE(3, 1, 2, 0));

    result.set(_mm_cvtss_f32(_mm_dp_ps(i_left_vec4.vec_, col1, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(i_left_vec4.vec_, col2, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(i_left_vec4.vec_, col3, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(i_left_vec4.vec_, col4, 0xFF)));

    return result;
}

// this * i_right_vec4
Vec4D Mat44::MultiplyRight(const Vec4D& i_right_vec4) const
{
    Vec4D result;

    result.set(_mm_cvtss_f32(_mm_dp_ps(row1, i_right_vec4.vec_, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row2, i_right_vec4.vec_, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row3, i_right_vec4.vec_, 0xFF)),
        _mm_cvtss_f32(_mm_dp_ps(row4, i_right_vec4.vec_, 0xFF)));

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
    return Mat44(_mm_setr_ps(1.0, 0.0f, 0.0f, i_translation.x()),
        _mm_setr_ps(0.0f, 1.0f, 0.0f, i_translation.y()),
        _mm_setr_ps(0.0f, 0.0f, 1.0f, i_translation.z()),
        _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

Mat44 Mat44::GetRotationX(const float i_radians)
{
    ASSERT(!IsNaN(i_radians));
    return Mat44(_mm_setr_ps(1.0, 0.0f, 0.0f, 0.0f),
        _mm_setr_ps(0.0f, cosf(i_radians), -sinf(i_radians), 0.0f),
        _mm_setr_ps(0.0f, sinf(i_radians), cosf(i_radians), 0.0f),
        _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

Mat44 Mat44::GetRotationY(const float i_radians)
{
    ASSERT(!IsNaN(i_radians));
    return Mat44(_mm_setr_ps(cosf(i_radians), 0, sinf(i_radians), 0.0f),
        _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f),
        _mm_setr_ps(-sinf(i_radians), 0.0f, cosf(i_radians), 0.0f),
        _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

Mat44 Mat44::GetRotationZ(const float i_radians)
{
    ASSERT(!IsNaN(i_radians));
    return Mat44(_mm_setr_ps(cosf(i_radians), -sinf(i_radians), 0.0f, 0.0f),
        _mm_setr_ps(sinf(i_radians), cosf(i_radians), 0.0f, 0.0f),
        _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f),
        _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

Mat44 Mat44::GetScale(const float i_scale)
{
    ASSERT(!IsNaN(i_scale));
    return Mat44(_mm_setr_ps(i_scale, 0.0f, 0.0f, 0.0f),
        _mm_setr_ps(0.0f, i_scale, 0.0f, 0.0f),
        _mm_setr_ps(0.0f, 0.0f, i_scale, 0.0f),
        _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

} // namespace optimized
} // namespace math
} // namespace engine

