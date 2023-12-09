#ifndef SA_DYNAMIC_ARRAY
    #define SA_DYNAMIC_ARRAY
    #include "SA/global/global.h"

    #define SA_DYNARRAY_DEFAULT_SIZE 256

    /*
    Create a DynamicArray for the type specified
    It returns a SA_DynamicArray*, which can be passed to other SA_dynarray... functions.
    */
    #define SA_dynarray_create(type) _SA_dynarray_create(sizeof(type), SA_DYNARRAY_DEFAULT_SIZE)

    /*
    Create a DynamicArray for the type specified
    The default size arguments is a hint to avoid to many reallocations if you know approximatively the size of the final array.
    The bigger this parameter is the faster it is to reallocate but it can also take more RAM than needed.
    It returns a SA_DynamicArray*, which can be passed to other SA_dynarray... functions.
    */
    #define SA_dynarray_create_size_hint(type, default_array_size) _SA_dynarray_create(sizeof(type), default_array_size)

    // this strange syntax is to be able to use const like 123 or "azerty"
    // if the value is not constant, the compiler will optimize away the variable v (I have verified in the asm generated)
    
    /*
    Set the value of a DynamicArray's column at the INDEX position.
    If the index is bigger than the size of the array, the array is extended and the values between are filled with garbage.
    TYPE: should be the same as specified in SA_dynarray_create.
    DYN_ARRAY: should be the SA_DynamicArray* pointer returned by SA_dynarray_create.
    INDEX: an uint64_t which is the index of the column you want to set.
    VALUE: The variable you want to put in the DynamicArray. should be the same type as TYPE.
    */
    #define SA_dynarray_set(type, dyn_array, index, value) {type _SA_v_0x382fa5612 = value; _SA_dynarray_set(dyn_array, index, &(_SA_v_0x382fa5612));}
    
    /*
    Append the value to the DynamicArray.
    TYPE: should be the same as specified in SA_dynarray_create.
    DYN_ARRAY: should be the SA_DynamicArray* pointer returned by SA_dynarray_create.
    VALUE: The variable you want to put in the DynamicArray. should be the same type as TYPE.
    */
    #define SA_dynarray_append(type, dyn_array, value) {type _SA_v_0x382fa5612 = value; _SA_dynarray_append(dyn_array, &(_SA_v_0x382fa5612));}
    
    /*
    Insert a value at the INDEX position of a DynamicArray.
    If the index is bigger than the size of the array, the array is extended and the values between are filled with garbage.
    TYPE: should be the same as specified in SA_dynarray_create.
    DYN_ARRAY: should be the SA_DynamicArray* pointer returned by SA_dynarray_create.
    INDEX: an uint64_t which is the index we want to insert your value.
    VALUE: The variable you want to put in the DynamicArray. should be the same type as TYPE.

    This is very costly, if you have multiple values to insert, use SA_dynarray_insert_uinitiliazed_block to create the space required,
    then, use SA_dynarray_set to fill the space with your values.
    */
    #define SA_dynarray_insert(type, dyn_array, index, value) {type _SA_v_0x382fa5612 = value; _SA_dynarray_insert(dyn_array, index, &(_SA_v_0x382fa5612));}
    
    /*
    Returns the value at the INDEX position of a DynamicArray.
    /!\ WARNING /!\: If the index is bigger than the size of the array, you will have a segfault
    TYPE: should be the same as specified in SA_dynarray_create.
    DYN_ARRAY: should be the SA_DynamicArray* pointer returned by SA_dynarray_create.
    INDEX: an uint64_t which is the index of the column you want to read.
    */
    #define SA_dynarray_get(type, dyn_array, index) (*((type*) _SA_dynarray_get_element_ptr(dyn_array, index)))

    typedef struct _SA_dynamic_array SA_DynamicArray;

    #ifdef __cplusplus
    extern "C"{
    #endif


    SA_bool SA_dynarray_insert_uninitialized_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements);
    SA_bool SA_dynarray_remove_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements);
    uint64_t SA_dynarray_size(const SA_DynamicArray* dyn_array);
    uint32_t SA_dynarray_get_element_size(const SA_DynamicArray* dyn_array);
    void SA_dynarray_qsort(SA_DynamicArray* dyn_array, int (*cmp_func)(const void*, const void*));
    void SA_dynarray_free(SA_DynamicArray** dyn_array);
    void SA_activate_zero_filling(SA_DynamicArray* dyn_array);

    // Internals

    SA_DynamicArray* _SA_dynarray_create(uint32_t element_size, uint32_t default_array_size);
    void _SA_dynarray_set(SA_DynamicArray* dyn_array, uint64_t index, void* value_ptr);
    void* _SA_dynarray_get_element_ptr(const SA_DynamicArray* dyn_array, uint64_t index);
    void _SA_dynarray_append(SA_DynamicArray* dyn_array, void* value_ptr);
    void _SA_dynarray_insert(SA_DynamicArray* dyn_array, uint64_t index, void* value_ptr);

    #ifdef __cplusplus
    }
    #endif
#endif