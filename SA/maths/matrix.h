#ifndef SA_MATRIX
    #define SA_MATRIX
    #include "SA/global/global.h"

    #ifdef SA_MATRIX_DOUBLE_PRECISION
        /*
        This is the type used in matrix.
        Currently it's a double but you can lower the precision to save memory.
        To do that, compile the lib without the SA_MATRIX_DOUBLE_PRECISION #define
        */
        typedef double SA_mat_float;
    #else
        /*
        This is the type used in matrix.
        Currently it's a float but you can increase the precision to a double.
        To do that, compile the lib with the SA_MATRIX_DOUBLE_PRECISION #define
        */
        typedef float SA_mat_float;
    #endif

    typedef struct _SA_matrix SA_Matrix;

    #ifdef __cplusplus
    extern "C"{
    #endif

    SA_Matrix* SA_WARN_UNUSED_RESULT SA_matrix_create(uint32_t row_size, uint32_t col_size);
    void SA_matrix_add_line_from_array(SA_Matrix* mat, uint32_t row, SA_mat_float* array);
    SA_Matrix* SA_WARN_UNUSED_RESULT SA_matrix_transpose(SA_Matrix* mat);
    SA_Matrix* SA_WARN_UNUSED_RESULT SA_Matrix_copy(SA_Matrix* mat);
    SA_bool SA_matrix_add(SA_Matrix* mat_dest, SA_Matrix* mat_add);
    void SA_matrix_print(SA_Matrix* mat);
    void SA_matrix_free(SA_Matrix** mat);

    #ifdef __cplusplus
    }
    #endif
#endif