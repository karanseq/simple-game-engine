// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Math\Mat44.h"
#include "Math\Vec4D.h"

#include "Math\Transform.h"

void TestMat44()
{
    LOG("-------------------- Running 4x4 Matrix Test --------------------");

    using namespace engine::math;

    // test default ctor
    Mat44 invalid_mat;
    ASSERT(!invalid_mat.IsValid());

    // test regular ctor
    Mat44 mat_01(1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f);
    // test operator bool
    ASSERT(mat_01);
    // test equality operator
    ASSERT(mat_01 == Mat44::IDENTITY);

    // test Set()
    mat_01.Set(1.0f, 2.0f, 3.0f, 4.0f,
               1.0f, 2.0f, 3.0f, 4.0f,
               1.0f, 2.0f, 3.0f, 4.0f,
               1.0f, 2.0f, 3.0f, 4.0f);

    /********** DETERMINANT TEST START **********/
    // determinant test 1
    float determinant_01 = mat_01.GetDeterminant();
    ASSERT(FuzzyEquals(determinant_01, 0.0f));

    Mat44 mat_02(2.0f, 5.0f, 1.0f, 9.0f,
                               4.0f, 6.0f, 1.0f, 2.0f,
                               6.0f, 1.0f, 6.0f, 2.0f,
                               8.0f, 8.0f, 2.0f, 3.0f);

    // determinant test 2
    float determinant_02 = mat_02.GetDeterminant();
    ASSERT(FuzzyEquals(determinant_02, -532.0f));
    /********** DETERMINANT TEST END **********/

    /********** MULTIPLICATION TEST START **********/
    // AI = A
    Mat44 mat_01_I(mat_01 * Mat44::IDENTITY);
    ASSERT(mat_01 == mat_01_I);

    // AB != BA
    Mat44 mat_01_02(mat_01 * mat_02);
    Mat44 mat_02_01(mat_02 * mat_01);
    ASSERT(!(mat_01_02 == mat_02_01));

    // V = M * V
    Vec4D vec4(10.f, 30.f, 20.f, 0.0f);
    Mat44 mat_rot_z = engine::math::Mat44::GetRotationZ(M_PI);
    engine::math::Vec4D vec_right = mat_rot_z * vec4;
    // V = V * M
    engine::math::Vec4D vec_left = vec4 * mat_rot_z;
    ASSERT(vec_left == vec_right);

    engine::math::Vec4D vec_rotated = vec4 * mat_rot_z;
    engine::math::Vec4D vec_unrotated = vec_rotated * engine::math::Mat44::GetRotationZ(-M_PI);
    ASSERT(vec4 == vec_unrotated);
    /********** MULTIPLICATION TEST END **********/

    /********** TRANSPOSE TEST START **********/
    // transpose test 1
    engine::math::Mat44 mat_01_transpose = mat_01.GetTranspose();
    engine::math::Mat44 check_mat_01_transpose(1.0f, 1.0f, 1.0f, 1.0f,
        2.0f, 2.0f, 2.0f, 2.0f,
        3.0f, 3.0f, 3.0f, 3.0f,
        4.0f, 4.0f, 4.0f, 4.0f);
    ASSERT(mat_01_transpose == check_mat_01_transpose);

    // (AB)' = B'A'
    engine::math::Mat44 mat_01_02_transpose(mat_01_02.GetTranspose());
    engine::math::Mat44 mat_02tr_01tr(mat_02.GetTranspose() * mat_01_transpose);
    ASSERT(mat_01_02_transpose == mat_02tr_01tr);

    // (A')' = A
    engine::math::Mat44 mat_01tr_transpose(mat_01_transpose.GetTranspose());
    ASSERT(mat_01tr_transpose == mat_01);
    /********** TRANSPOSE TEST START **********/

    /********** INVERSE TEST START **********/
    engine::math::Mat44 mat_01_inverse = mat_01.GetInverse();
    ASSERT(mat_01_inverse == mat_01);

    // inverse test 2
    engine::math::Mat44 mat_02_inverse = mat_02.GetInverse();
    engine::math::Mat44 check_mat_02_inverse(-0.009398f, -0.622180f, -0.054511f,  0.479323f,
                                             -0.033835f,  0.560150f,  0.003759f, -0.274436f,
                                             -0.030075f,  0.609023f,  0.225564f, -0.466165f,
                                              0.135338f, -0.240602f, -0.015038f,  0.097744f);
    ASSERT(mat_02_inverse == check_mat_02_inverse);

    // A * A^ = I
    engine::math::Mat44 mat_02_mult_01 = mat_02 * mat_02_inverse;
    ASSERT(mat_02_mult_01 == engine::math::Mat44::IDENTITY);

    // A^ * A = I
    engine::math::Mat44 mat_02_mult_02 = mat_02_inverse * mat_02;
    ASSERT(mat_02_mult_02 == engine::math::Mat44::IDENTITY);

    // (A^)^ = A
    engine::math::Mat44 mat_02in_inverse(mat_02_inverse.GetInverse());
    ASSERT(mat_02in_inverse == mat_02);

    engine::math::Mat44 mat_03(1.0f, -5.0f, 9.0f, 2.0f,
                               -3.0f, 6.0f, -4.0f, -9.0f,
                               -6.0f, -2.0f, 0.0f, 7.0f,
                               11.0f, 5.0f, 8.0f, 4.0f);

    // (AB)^ = B^A^
    engine::math::Mat44 mat_02_03_inverse((mat_02 * mat_03).GetInverse());
    engine::math::Mat44 mat_03in_02in(mat_03.GetInverse() * mat_02_inverse);
    ASSERT(mat_02_03_inverse == mat_03in_02in);

    engine::math::Mat44 mat_03_tr_in(mat_03.GetTranspose().GetInverse());
    engine::math::Mat44 mat_03_in_tr(mat_03.GetInverse().GetTranspose());
    ASSERT(mat_03_tr_in == mat_03_in_tr);
    /********** INVERSE TEST END **********/

    /********** TRANSFORMATIONS TEST START **********/
    const engine::math::Vec4D           some_point(1.0f, 0.0f, 0.0f, 1.0f);
    const engine::math::Mat44           mat_rotate(engine::math::Mat44::GetRotationZ(M_PI / 2));
    const engine::math::Mat44           mat_translate(engine::math::Mat44::GetTranslation(engine::math::Vec3D(0.0f, 10.0f, 0.0f)));
    
    const engine::math::Mat44           mat_transform(mat_translate * mat_rotate);
    const engine::math::Mat44           mat_undo_transform(mat_transform.GetInverse());

    const engine::math::Vec4D           rotated_point(mat_rotate * some_point);
    ASSERT(rotated_point == engine::math::Vec4D(0.0f, 1.0f, 0.0f, 1.0f));

    const engine::math::Vec4D           translated_point(mat_translate * some_point);
    ASSERT(translated_point == engine::math::Vec4D(1.0f, 10.0f, 0.0f, 1.0f));

    const engine::math::Vec4D           transformed_point(mat_transform * some_point);
    ASSERT(transformed_point == engine::math::Vec4D(0.0f, 11.0f, 0.0f, 1.0f));

    const engine::math::Vec4D           original_point(mat_undo_transform * transformed_point);
    ASSERT(original_point == some_point);
    /********** TRANSFORMATIONS TEST END **********/

    LOG("-------------------- Finished 4x4 Matrix Test --------------------");
}