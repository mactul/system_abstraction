#include "SA/memory/mem_alloc.h"
#include "SA/strings/strings.h"
#include "SA/memory/dynamic_array.h"

#define DEFAULT_SIZE 256

struct _SA_dynamic_array {
    SA_byte* elements;
    uint32_t element_size;
    uint64_t nb_elements;
    uint64_t nb_slots;
};

/*
Returns the size of the type used in the array.
*/
uint32_t SA_dynarray_get_element_size(SA_DynamicArray* dyn_array)
{
    return dyn_array->element_size;
}

/*
Returns the number of elements in the array.
*/
uint64_t SA_dynarray_size(SA_DynamicArray* dyn_array)
{
    return dyn_array->nb_elements;
}

static SA_bool SA_dynarray_ensure_nb_slots(SA_DynamicArray* dyn_array, uint64_t nb_slots)
{
    if(nb_slots > dyn_array->nb_slots)
    {
        SA_byte* temp_elements;
        while(nb_slots > dyn_array->nb_slots)  // This loop will probably only be executed once.
        {
            if(dyn_array->nb_slots >= (1ULL << (8*sizeof(dyn_array->nb_slots)-1)))
            {
                // We can't double this without an overflow.
                // at this moment, the memory sould be completly saturated, but who knows...
                return SA_FALSE;
            }
            dyn_array->nb_slots <<= 1;  // We use the double of the actual size
        }
        temp_elements = SA_realloc(dyn_array->elements, dyn_array->element_size * dyn_array->nb_slots);
        if(temp_elements == NULL)
        {
            return SA_FALSE;
        }
        dyn_array->elements = temp_elements;
    }
    return SA_TRUE;
}

/*
Insert a space of NB_BLOCK_ELEMENTS in the DynamicArray.
The space created is uinitiliazed, so filled with garbage.
*/
SA_bool SA_dynarray_insert_uinitiliazed_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements)
{
    if(!SA_dynarray_ensure_nb_slots(dyn_array, dyn_array->nb_elements+nb_block_elements))
    {
        return SA_FALSE;
    }
    dyn_array->nb_elements += nb_block_elements;
    
    for(uint64_t i = dyn_array->nb_elements-1; i > index+nb_block_elements-1; i--)
    {
        uint64_t n = i * dyn_array->element_size;
        dyn_array->elements[n] = dyn_array->elements[n - nb_block_elements * dyn_array->element_size];
    }
    return SA_TRUE;
}

/*
Remove NB_BLOCK_ELEMENTS from the DynamicArray starting at position INDEX.
If nothing was removed, the function returns SA_FALSE, else it returns SA_TRUE.

This doesn't free any memory, the remaining memory is used for futur use in the DynamicArray.
*/
SA_bool SA_dynarray_remove_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements)
{
    if(index >= dyn_array->nb_elements)
    {
        return SA_FALSE;
    }
    if(dyn_array->nb_elements-index < nb_block_elements)
    {
        dyn_array->nb_elements = index;
        return SA_TRUE;
    }
    
    dyn_array->nb_elements -= nb_block_elements;
    
    for(uint64_t i = index; i < dyn_array->nb_elements; i++)
    {
        uint64_t n = i * dyn_array->element_size;
        dyn_array->elements[n] = dyn_array->elements[n + nb_block_elements * dyn_array->element_size];
    }
    return SA_TRUE;
}

SA_DynamicArray* _SA_dynarray_create(uint32_t element_size)
{
    SA_DynamicArray* dyn_array = (SA_DynamicArray*) SA_malloc(sizeof(SA_DynamicArray));
    if(dyn_array == NULL)
    {
        return NULL;
    }
    dyn_array->elements = (SA_byte*) SA_malloc(element_size*DEFAULT_SIZE);
    if(dyn_array->elements == NULL)
    {
        SA_free(&dyn_array);
        return NULL;
    }
    dyn_array->element_size = element_size;
    dyn_array->nb_elements = 0;
    dyn_array->nb_slots = DEFAULT_SIZE;

    return dyn_array;
}

void _SA_dynarray_set(SA_DynamicArray* dyn_array, uint64_t index, void* value_ptr)
{
    SA_dynarray_ensure_nb_slots(dyn_array, index+1);
    SA_memcpy(dyn_array->elements + index * dyn_array->element_size, value_ptr, sizeof(SA_byte) * dyn_array->element_size);
}

void* _SA_dynarray_get_element_ptr(SA_DynamicArray* dyn_array, uint64_t index)
{
    if(index >= dyn_array->nb_elements)
    {
        return NULL;  // This will generate a segfault with the macro.
    }
    return dyn_array->elements + index * dyn_array->element_size;
}

void _SA_dynarray_append(SA_DynamicArray* dyn_array, void* value_ptr)
{
    _SA_dynarray_set(dyn_array, dyn_array->nb_elements, value_ptr);
    dyn_array->nb_elements++;
}

void _SA_dynarray_insert(SA_DynamicArray* dyn_array, uint64_t index, void* value_ptr)
{
    if(!SA_dynarray_insert_uinitiliazed_block(dyn_array, index, 1))
    {
        return;
    }
    _SA_dynarray_set(dyn_array, index, value_ptr);
}

/*
Free the underlying DynamicArray behind the pointer DYN_ARRAY and set the DynamicArray handler to NULL.
*/
void SA_dynarray_free(SA_DynamicArray** dyn_array)
{
    if(*dyn_array == NULL)
    {
        return;
    }
    SA_free(&((*dyn_array)->elements));
    SA_free(dyn_array);
}