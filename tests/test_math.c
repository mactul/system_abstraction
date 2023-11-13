#include "test.h"

void test_math(void)
{
    SA_Matrix* mat1 = NULL;
    SA_Matrix* mat2 = NULL;
    SA_mat_float array[5] = {1, 2, 3, 4, 5};
    SA_mat_float array2[5] = {6792.6, 2, 3, 4, 5};

    assert(SA_int_pow(5, 7) == 78125);
    assert(SA_int_pow(5, 0) == 1);
    assert(SA_int_pow(0, 7) == 0);
    assert(SA_int_pow(0, 0) == 1);

    assert(SA_RELU(5) == 5);
    assert(SA_RELU(-5) == 0);
    assert(SA_RELU(6.7) == 6.7);

    for(int i = 0; i < 100; i++)
    {
        double x = SA_logistic(SA_random_unsecure_float(-1000, 1000));
        assert(x >= 0 && x <= 1.0);
    }
    for(int i = 0; i < 100; i++)
    {
        double x = SA_logistic(SA_random_unsecure_float(-10, 10));
        assert(x > 0 && x < 1.0);
    }


    mat1 = SA_matrix_create(3, 5);

    SA_matrix_add_line_from_array(mat1, 0, array);
    SA_matrix_add_line_from_array(mat1, 1, array2);
    SA_matrix_add_line_from_array(mat1, 2, array);

    SA_matrix_print(mat1);

    mat2 = SA_matrix_transpose(mat1);

    SA_matrix_print(mat2);

    SA_matrix_free(&mat1);
    SA_matrix_free(&mat2);
}