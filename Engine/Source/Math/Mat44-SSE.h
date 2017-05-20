#ifndef ENGINE_MAT44_SSE_H_
#define ENGINE_MAT44_SSE_H_

// library includes
#include <xmmintrin.h>
#include <smmintrin.h>

// engine includes
#include "Vec4D-SSE.h"

namespace engine {
namespace math {
namespace optimized {

class Mat44
{
public:
    explicit Mat44();
    Mat44(float i_11, float i_12, float i_13, float i_14,
        float i_21, float i_22, float i_23, float i_24,
        float i_31, float i_32, float i_33, float i_34,
        float i_41, float i_42, float i_43, float i_44);
    Mat44(const __m128& i_row1, const __m128& i_row2, const __m128& i_row3, const __m128& i_row4);

    ~Mat44();

    void Set(float i_11, float i_12, float i_13, float i_14,
        float i_21, float i_22, float i_23, float i_24,
        float i_31, float i_32, float i_33, float i_34,
        float i_41, float i_42, float i_43, float i_44);

    void Set(const __m128& i_row1, const __m128& i_row2, const __m128& i_row3, const __m128& i_row4);

    // copy constructor
    Mat44(const Mat44& i_copy);
    // copy assignment operator
    Mat44& operator=(const Mat44& i_other);

    // validity check
    inline bool IsValid() const;

    // get the determinant
    float GetDeterminant() const;

    // invert this matrix
    void Invert();
    // return an inverse of this matrix
    Mat44 GetInverse() const;

    // transpose this matrix
    void Transpose();
    // return a transpose of this matrix
    Mat44 GetTranspose() const;

    // multiply this matrix with a scalar
    //Mat44 operator*(float i_scalar);
    // equality check
    bool operator==(const Mat44& i_other) const;
    // validity check
    inline operator bool() const;

    // multiply a matrix with this matrix viz. this * i_right_mat
    Mat44 Multiply(const Mat44& i_right_mat) const;
    // multiply a Vec4D with this matrix viz. i_vec4 * this
    Vec4D MultiplyLeft(const Vec4D& i_left_vec4) const;
    // multiply a Vec4D with this matrix viz. this * i_vec4
    Vec4D MultiplyRight(const Vec4D& i_right_vec4) const;

    // translation matrix
    static Mat44 GetTranslation(const Vec3D& i_translation);

    // rotation matrices
    static Mat44 GetRotationX(const float i_radians);
    static Mat44 GetRotationY(const float i_radians);
    static Mat44 GetRotationZ(const float i_radians);

    // scaling matrix
    static Mat44 GetScale(const float i_scale);

#ifdef BUILD_DEBUG
    void Print() const;
#endif

    static const Mat44 IDENTITY;
    static const Mat44 ZERO;

private:
    union
    {
        struct
        {
            float       f11, f12, f13, f14;
            float       f21, f22, f23, f24;
            float       f31, f32, f33, f34;
            float       f41, f42, f43, f44;
        };
        struct
        {
            __m128          row1, row2, row3, row4;
        };
    };

}; // class Mat44

   // matrix multiplication
inline Mat44 operator*(const Mat44& i_left, const Mat44& i_right);
// V = M * V
inline Vec4D operator*(const Mat44& i_left, const Vec4D& i_right);
// V = V * M
inline Vec4D operator*(const Vec4D& i_left, const Mat44& i_right);

} // namespace optimized
} // namespace math
} // namespace engine

#include "Mat44-SSE-inl.h"

#endif // ENGINE_MAT44_SSE_H_
