#include <stdlib.h>
#include "SA/memory/mem_alloc.h"
#include "SA/strings/strings.h"
#include "SA/memory/dynamic_array.h"

struct _SA_dynamic_array
{
    uint64_t nb_elements;
    uint64_t nb_slots;
    SA_byte *elements;
    uint32_t element_size;
    SA_bool init_to_zero;
};

/*
Returns the size of the type used in the array.
*/
uint32_t SA_dynarray_get_element_size(const SA_DynamicArray *dyn_array)
{
    return dyn_array->element_size;
}

/*
Returns the number of elements in the array.
*/
uint64_t SA_dynarray_size(const SA_DynamicArray *dyn_array)
{
    return dyn_array->nb_elements;
}

static SA_bool SA_dynarray_ensure_nb_slots(SA_DynamicArray *dyn_array, uint64_t nb_slots)
{
    if (nb_slots > dyn_array->nb_slots)
    {
        SA_byte *temp_elements;
        while (nb_slots > dyn_array->nb_slots) // This loop will probably only be executed once.
        {
            if (dyn_array->nb_slots >= (1ULL << (8 * sizeof(dyn_array->nb_slots) - 1)))
            {
                // We can't double this without an overflow.
                // at this moment, the memory sould be completly saturated, but who knows...
                _SA_set_error(SA_ERROR_OVERFLOW);
                return SA_FALSE;
            }
            dyn_array->nb_slots <<= 1; // We use the double of the actual size
        }
        temp_elements = SA_realloc(dyn_array->elements, dyn_array->element_size * dyn_array->nb_slots);
        if (temp_elements == NULL)
        {
            _SA_set_error(SA_ERROR_MALLOC);
            return SA_FALSE;
        }
        dyn_array->elements = temp_elements;
        if (dyn_array->init_to_zero)
        {
            void *ptr = dyn_array->elements + dyn_array->element_size * dyn_array->nb_elements;
            uint64_t size = (dyn_array->nb_slots - dyn_array->nb_elements) * dyn_array->element_size;
            SA_memset(ptr, 0, size);
        }
    }
    return SA_TRUE;
}

/*
Insert a space of NB_BLOCK_ELEMENTS in the DynamicArray.
The space created is uinitiliazed, so filled with garbage.
*/
SA_bool SA_dynarray_insert_uninitialized_block(SA_DynamicArray *dyn_array, uint64_t index, uint64_t nb_block_elements)
{
    if (!SA_dynarray_ensure_nb_slots(dyn_array, dyn_array->nb_elements + nb_block_elements))
    {
        return SA_FALSE;
    }
    dyn_array->nb_elements += nb_block_elements;

    for (uint64_t i = (dyn_array->nb_elements-1) * dyn_array->element_size; i > (index + nb_block_elements - 1) * dyn_array->element_size; i--)
    {
        dyn_array->elements[i] = dyn_array->elements[i - nb_block_elements * dyn_array->element_size];
    }
    return SA_TRUE;
}

/*
Remove NB_BLOCK_ELEMENTS from the DynamicArray starting at position INDEX.
If nothing was removed, the function returns SA_FALSE, else it returns SA_TRUE.

This doesn't free any memory, the remaining memory is used for futur use in the DynamicArray.
*/
SA_bool SA_dynarray_remove_block(SA_DynamicArray *dyn_array, uint64_t index, uint64_t nb_block_elements)
{
    if (index >= dyn_array->nb_elements)
    {
        return SA_FALSE;
    }
    if (dyn_array->nb_elements - index < nb_block_elements)
    {
        dyn_array->nb_elements = index;
        return SA_TRUE;
    }

    SA_memcpy(dyn_array->elements + index * dyn_array->element_size, dyn_array->elements + (index + nb_block_elements) * dyn_array->element_size, (dyn_array->nb_elements - index - nb_block_elements) * dyn_array->element_size);

    dyn_array->nb_elements -= nb_block_elements;

    return SA_TRUE;
}

/*
Sort the dynamic array by comparing each element with the CMP_FUNC callback
*/
void SA_dynarray_qsort(SA_DynamicArray *dyn_array, int (*cmp_func)(const void *, const void *))
{
    qsort(dyn_array->elements, dyn_array->nb_elements, dyn_array->element_size, cmp_func);
}

void SA_activate_zero_filling(SA_DynamicArray *dyn_array)
{
    dyn_array->init_to_zero = SA_TRUE;
}

SA_DynamicArray *_SA_dynarray_create(uint32_t element_size, uint32_t default_array_size)
{
    SA_DynamicArray *dyn_array = (SA_DynamicArray *)SA_calloc(sizeof(SA_DynamicArray));
    if (dyn_array == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return NULL;
    }
    dyn_array->elements = (SA_byte *)SA_calloc(default_array_size * element_size);
    if (dyn_array->elements == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        SA_free(&dyn_array);
        return NULL;
    }
    dyn_array->element_size = element_size;
    dyn_array->nb_elements = 0;
    dyn_array->nb_slots = default_array_size;
    dyn_array->init_to_zero = SA_FALSE;

    return dyn_array;
}

void _SA_dynarray_set(SA_DynamicArray *dyn_array, uint64_t index, void *value_ptr)
{
    if (!SA_dynarray_ensure_nb_slots(dyn_array, index + 1))
    {
        return;
    }
    if (dyn_array->nb_elements <= index)
    {
        dyn_array->nb_elements = index + 1;
    }
    SA_memcpy(dyn_array->elements + index * dyn_array->element_size, value_ptr, sizeof(SA_byte) * dyn_array->element_size);
}

void *_SA_dynarray_get_element_ptr(const SA_DynamicArray *dyn_array, uint64_t index)
{
    if (index >= dyn_array->nb_elements)
    {
        return NULL; // This will generate a segfault with the macro.
    }
    return dyn_array->elements + index * dyn_array->element_size;
}

void _SA_dynarray_append(SA_DynamicArray *dyn_array, void *value_ptr)
{
    _SA_dynarray_set(dyn_array, dyn_array->nb_elements, value_ptr);
}

void _SA_dynarray_insert(SA_DynamicArray *dyn_array, uint64_t index, void *value_ptr)
{
    if (!SA_dynarray_insert_uninitialized_block(dyn_array, index, 1))
    {
        return;
    }
    _SA_dynarray_set(dyn_array, index, value_ptr);
}

/*
Free the underlying DynamicArray behind the pointer DYN_ARRAY and set the DynamicArray handler to NULL.
*/
void SA_dynarray_free(SA_DynamicArray **dyn_array)
{
    if (*dyn_array == NULL)
    {
        return;
    }
    SA_free(&((*dyn_array)->elements));
    SA_free(dyn_array);
}
