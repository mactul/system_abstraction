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

    /**
     * @brief Allocate a new matrix of size `row_size` * `col_size`.  
     * @brief All elements of the matrix are set to 0 by default.  
     * @brief After the use of this matrix, you have to free it with `SA_matrix_free`.
     * @param row_size the number of rows in the matrix
     * @param col_size the number of cols in the matrix
     * @return A handler to a matrix object
    */
    SA_Matrix* SA_WARN_UNUSED_RESULT SA_matrix_create(uint32_t row_size, uint32_t col_size);

    /**
     * @brief Fill the line n°`row` of the matrix `mat` with the elements of `array`.
     * @brief `array` must be as long as the number of columns in mat.
     * @param mat The matrix handler to modify
     * @param row The index of the row to fill
     * @param array The array where to pick elements
    */
    void SA_matrix_set_line_from_array(SA_Matrix* mat, uint32_t row, const SA_mat_float* array);

    /**
     * @brief Transpose the matrix `mat` in a new matrix
     * @param mat The matrix to transpose
     * @return A new matrix which is the transposed matrix of `mat`
    */
    SA_Matrix* SA_WARN_UNUSED_RESULT SA_matrix_transpose(const SA_Matrix* mat);

    /**
     * @brief Create a new matrix which is the exact copy of `mat`
     * @param mat The matrix to copy
     * @return A copy of `mat`
    */
    SA_Matrix* SA_WARN_UNUSED_RESULT SA_Matrix_copy(const SA_Matrix* mat);

    /**
     * @brief Add mat_dest and mat_add into mat_dest.
     * @param mat_dest The destination matrix.
     * @param mat_add The matrix to add to mat_dest.
     * @return Whether or not the operation can be done.
    */
    SA_bool SA_matrix_add(SA_Matrix* mat_dest, const SA_Matrix* mat_add);

    /**
     * @brief Display the matrix in a readable way.
     * @brief Use it in debug purpose.
     * @param mat The matrix to display
    */
    void SA_matrix_print(const SA_Matrix* mat);

    /**
     * @brief Free the underlying matrix behind the pointer `mat` and set the matrix handler to NULL.
     * @param mat The address of the handler returned by `SA_matrix_create` or a copy function.
    */
    void SA_matrix_free(SA_Matrix** mat);

    #ifdef __cplusplus
    }
    #endif
#endif