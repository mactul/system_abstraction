#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "SA/maths/matrix.h"
#include "SA/memory/mem_alloc.h"


struct _SA_matrix {
    uint32_t row_size;
    uint32_t col_size;
    SA_mat_float* elements;
};


/*
Allocate a new matrix of size ROW_SIZE * COL_SIZE.
All elements of the matrix are set to 0 by default.
After the use of this matrix, you have to free it with `SA_matrix_free`.
*/
SA_Matrix* SA_matrix_create(uint32_t row_size, uint32_t col_size)
{
    SA_Matrix* mat = (SA_Matrix*) SA_malloc(sizeof(SA_Matrix));
    if(mat == NULL)
    {
        return NULL;
    }
    mat->row_size = row_size;
    mat->col_size = col_size;

    mat->elements = (SA_mat_float*) SA_calloc(sizeof(SA_mat_float)*row_size*sizeof(SA_mat_float)*col_size);
    if(mat == NULL)
    {
        return NULL;
    }
    return mat;
}

/*
Fill the line n°ROW of the matrix MAT with the elements of ARRAY.
ARRAY must be as long as the number of columns in mat.
*/
void SA_matrix_set_line_from_array(SA_Matrix* mat, uint32_t row, const SA_mat_float* array)
{
    SA_mat_float* line = mat->elements + row * mat->col_size;
    for(uint32_t i = 0; i < mat->col_size; i++)
    {
        line[i] = array[i];
    }
}

/*
Returns a new matrix which is the transposition of the matrix mat.
This new matrix must be freed by `SA_matrix_free`.
*/
SA_Matrix* SA_matrix_transpose(const SA_Matrix* mat)
{
    SA_Matrix* out_mat = SA_matrix_create(mat->col_size, mat->row_size);
    if(out_mat == NULL)
    {
        return NULL;
    }
    for(uint32_t i = 0; i < mat->row_size; i++)
    {
        for(uint32_t j = 0; j < mat->col_size; j++)
        {
            out_mat->elements[j * out_mat->col_size + i] = mat->elements[i * mat->col_size + j];
        }
    }
    return out_mat;
}

/*
Create a new matrix which is a copy of the matrix MAT.
This new matrix must be freed by `SA_matrix_free`.
*/
SA_Matrix* SA_Matrix_copy(const SA_Matrix* mat)
{
    SA_Matrix* out_mat = SA_matrix_create(mat->row_size, mat->col_size);
    if(out_mat == NULL)
    {
        return NULL;
    }
    for(uint32_t i = 0; i < mat->row_size*mat->col_size; i++)
    {
        out_mat->elements[i] = mat->elements[i];
    }
    return out_mat;
}

/*
Add the content of the matrix MAT_ADD to the content of the matrix MAT_DEST
It returns SA_FALSE if the operation is not permitted (the matrix must have the same size)
*/
SA_bool SA_matrix_add(SA_Matrix* mat_dest, const SA_Matrix* mat_add)
{
    if(mat_add->col_size != mat_dest->col_size || mat_add->row_size != mat_dest->row_size)
    {
        return SA_FALSE;
    }
    for(uint32_t i = 0; i < mat_add->row_size*mat_add->col_size; i++)
    {
        mat_dest->elements[i] += mat_add->elements[i];
    }
    return SA_TRUE;
}

/*
This display the matrix in a readable way.
Use it in debug purpose.
*/
void SA_matrix_print(const SA_Matrix* mat)
{
    printf("┌");
    for(uint32_t i = 0; i < mat->col_size; i++)
    {
        printf("        ");
    }
    puts(" ┐");
    for(uint32_t i = 0; i < mat->row_size; i++)
    {
        SA_mat_float* line = mat->elements + i * mat->col_size;
        putchar('|');
        putchar(' ');
        for(uint32_t j = 0; j < mat->col_size; j++)
        {
            printf("%7.2f ", line[j]);
        }
        putchar('|');
        putchar('\n');
    }
    printf("└");
    for(uint32_t i = 0; i < mat->col_size; i++)
    {
        printf("        ");
    }
    puts(" ┘");
}

/*
Free the underlying matrix behind the pointer MAT and set the matrix handler to NULL.
*/
void SA_matrix_free(SA_Matrix** mat)
{
    if(*mat == NULL)
    {
        return;
    }
    SA_free(&((*mat)->elements));
    SA_free(mat);
}