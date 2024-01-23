#ifndef SA_DYNAMIC_ARRAY
    #define SA_DYNAMIC_ARRAY
    #include "SA/global/global.h"

    #define SA_DYNARRAY_DEFAULT_SIZE 256

    /**
     * @brief Create a DynamicArray for the type specified
     * @param type any type on which you can call `sizeof`, for example `int`, `float`, `struct vector`, etc... It will be the type used in the whole DynamicArray.
     * @return - when it succeeds, it returns a pointer to a SA_DynamicArray handler.
     * @return - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
    */
    #define SA_dynarray_create(type) _SA_dynarray_create(sizeof(type), SA_DYNARRAY_DEFAULT_SIZE)

    /**
     * @brief This function works exactly like `SA_dynarray_create`  
     * @brief In addition, you can pass the default size of the array. This size will be the first size of the memory allocated by the array and when this will no longer be sufficient, the array will be extended.
     * @param type any type on which you can call `sizeof`, for example `int`, `float`, `struct vector`, etc... It will be the type used in the whole DynamicArray.
     * @param default_array_size A hint for the compiler. The bigger this parameter is the faster it is to reallocate memory but it can also take more RAM than needed. You should set this parameter to the approximative final size you expect for the dynamic array.
     * @return - when it succeeds, it returns a pointer to a structure handler.
     * @return - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
    */
    #define SA_dynarray_create_size_hint(type, default_array_size) _SA_dynarray_create(sizeof(type), default_array_size)
    
    /*
    The strange syntax below is to be able to use const like 123 or "azerty".
    If the value is not constant, the compiler will optimize away the variable v (I have verified in the asm generated)
    */
    

    /**
     * @brief Set the value of a DynamicArray's column at the `index` position.  
     * @brief If the index is bigger than the size of the array, the array is extended and the values between are filled with garbage.
     * @brief If you want to fill the array with zeros instead of garbage, call `SA_dynarray_activate_zero_filling` before.
     * @param type the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the column you want to set.
     * @param value the value you want to set in the array, it's the type `type`.
     * @return It returns nothing, but it can fail by malloc error, in this case `SA_get_error` will return `SA_ERROR_MALLOC` or `SA_ERROR_OVERFLOW` (but this one is unlikely to happen)
    */
    #define SA_dynarray_set(type, dyn_array, index, value) {type _SA_v_0x382fa5612 = value; _SA_dynarray_set(dyn_array, index, &(_SA_v_0x382fa5612));}
    
    /**
     * @brief Append the value to the DynamicArray.
     * @param type the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param value the value you want to append to the array, it's the type `type`
     * @return It returns nothing, but it can fail by malloc error, in this case `SA_get_error` will return `SA_ERROR_MALLOC` or `SA_ERROR_OVERFLOW` (but this one is unlikely to happen)
    */
    #define SA_dynarray_append(type, dyn_array, value) {type _SA_v_0x382fa5612 = value; _SA_dynarray_append(dyn_array, &(_SA_v_0x382fa5612));}
    

    /**
     * @brief Insert a value at the `index` position of a DynamicArray.  
     * @brief If the index is bigger than the size of the array, the array is extended and the values between are filled with garbage.  
     * @brief  
     * @brief This is very costly, if you have multiple values to insert, use `SA_dynarray_insert_uinitiliazed_block` to create the space required.  
     * @brief Then, use `SA_dynarray_set` to fill the space with your values.
     * @param type the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the column where you want to insert your value.
     * @param value the value you want to insert in the array, it's the type `type`.
     * @return It returns nothing, but it can fail by malloc error, in this case `SA_get_error` will return `SA_ERROR_MALLOC` or `SA_ERROR_OVERFLOW` (but this one is unlikely to happen)
    */
    #define SA_dynarray_insert(type, dyn_array, index, value) {type _SA_v_0x382fa5612 = value; _SA_dynarray_insert(dyn_array, index, &(_SA_v_0x382fa5612));}

    /**
     * @brief Returns the value at position `index` in the array `dyn_array`.  
     * @brief /!\\ WARNING /!\\ : If the index is bigger than the size of the array, you will have a segfault.
     * @param type the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the column you want to get.
     * @return the value of the array's column.
    */
    #define SA_dynarray_get(type, dyn_array, index) (*((type*) _SA_dynarray_get_element_ptr(dyn_array, index)))

    // retrocompatibilty function - prefere using SA_dynarray_activate_zero_filling
    #define SA_activate_zero_filling(dyn_array) SA_dynarray_activate_zero_filling(dyn_array)


    typedef struct _SA_dynamic_array SA_DynamicArray;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief This will move the dyn_array elements to create a space of uninitialized elements.  
     * You should then set them using `SA_dynarray_set`.
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the column where you want to insert the uninitialized block.
     * @param nb_block_elements the number of elements in the uninitialized block.
     * @return - when it succeeds, it returns SA_TRUE;
     * @return - when it fails, it returns SA_FALSE and `SA_print_last_error()` can tell what happened
     */
    SA_bool SA_dynarray_insert_uninitialized_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements);
    
    /**
     * @brief This will move the dyn_array elements to remove `nb_block_elements` at position `index`.
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the column of the first element to remove.
     * @param nb_block_elements the number of elements in the uninitialized block.
     * @return - when it succeeds, it returns SA_TRUE;
     * @return - when it fails, it returns SA_FALSE. This can only happen if index is bigger than the size of the dyn_array.
     */
    SA_bool SA_dynarray_remove_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements);
    
    /**
     * @brief Returns the number of elements in the DynamicArray
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @return uint64_t
     */
    uint64_t SA_FUNCTION_PURE SA_dynarray_size(const SA_DynamicArray* dyn_array);

    /**
     * @brief Returns the size of the type used in the array.
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @return the size of the type used in `dyn_array`.
    */
    uint32_t SA_FUNCTION_PURE SA_dynarray_get_element_size(const SA_DynamicArray* dyn_array);

    /**
     * @brief Sort the dynamic array by comparing each element with the `cmp_func` callback.
     * 
     * @param dyn_array the address of the pointer returned by `SA_dynarray_create`.
     * @param cmp_func a callback to compare 2 elements. Should returns:
     * `0` if `*el1 == *el2`,
     * `< 0` if `*el1 < *el2`,
     * `> 0` if `*el1 > *el2`  
     */
    void SA_dynarray_qsort(SA_DynamicArray* dyn_array, int (*cmp_func)(const void*, const void*));

    /**
     * @brief Free the underlying DynamicArray behind the pointer dyn_array and set the DynamicArray handler to NULL.
     * 
     * @param dyn_array the address of the pointer returned by `SA_dynarray_create`.
     */
    void SA_dynarray_free(SA_DynamicArray** dyn_array);

    /**
     * @brief This function should be called after the creation of the array to specify that you want it to be filled with zeros when it expends.
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     */
    void SA_dynarray_activate_zero_filling(SA_DynamicArray* dyn_array);


    // Internals

    /**
     * @brief Internal function to create an array
     * 
     * @param element_size the size of an array's element (`sizeof(int)` for an array of `int`)
     * @param default_array_size the first size the array should allocate.
     * @return a DynamicArray handler
     */
    SA_DynamicArray* _SA_dynarray_create(uint32_t element_size, uint32_t default_array_size);

    /**
     * @brief Internal function to set an element of the array
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the element to set.
     * @param value_ptr the address of the value to copy in the array.
     */
    void _SA_dynarray_set(SA_DynamicArray* dyn_array, uint64_t index, void* value_ptr);

    /**
     * @brief Internal function to get the address of the array, starting at `index` element.
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the element you want to get.
     * @return the address to the element and by extension to the array behind or NULL if the element doesn't exists.
     */
    void* SA_FUNCTION_PURE _SA_dynarray_get_element_ptr(const SA_DynamicArray* dyn_array, uint64_t index);

    /**
     * @brief Internal function to append an element to the array.
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param value_ptr the address of the value to copy in the array.
     */
    void _SA_dynarray_append(SA_DynamicArray* dyn_array, void* value_ptr);

    /**
     * @brief Internal function to insert an element in a specific position of the array.
     * 
     * @param dyn_array the handler returned by `SA_dynarray_create`.
     * @param index the index of the position for the element to be inserted.
     * @param value_ptr the address of the value to copy in the array.
     */
    void _SA_dynarray_insert(SA_DynamicArray* dyn_array, uint64_t index, void* value_ptr);

    #ifdef __cplusplus
    }
    #endif
#endif