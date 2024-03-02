#ifndef SA_MEMORY
    #define SA_MEMORY
    #include "SA/global/global.h"

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_FUNCTION_NOTHROW _SA_free(void* ptr);

    /**
     * @brief Allocate `size` bytes of memory
     * 
     * @param size the number of bytes to allocate. 
     * @return The address of the memory allocated or NULL if an error happened.
     */
    void* SA_WARN_UNUSED_RESULT SA_FUNCTION_NOTHROW SA_malloc(size_t size) SA_MALLOC_FUNC(_SA_free);


    /**
     * @brief Allocate `size` bytes of memory and set them to zero.
     * 
     * @param size the number of bytes to allocate. 
     * @return The address of the memory allocated or NULL if an error happened.
     */
    void* SA_WARN_UNUSED_RESULT SA_calloc(size_t size);

    /**
     * @brief Add `size` bytes of memory in the previously allocated `ptr` block 
     * 
     * @param ptr a previously allocated area, allocated by malloc or calloc.
     * @param size The new size of the memory block. If it's less than the initial size, the memory will be deallocated.
     * @return void* 
     */
    void* SA_WARN_UNUSED_RESULT SA_realloc(void* ptr, size_t size);


    #ifdef SA_MEMORY_DEBUG
        SA_bool _SA_is_everything_freed(void);
    #endif

    #ifdef __cplusplus
    }
    #endif

    /**
    * @brief Internal function  
    * @brief free the memory block and set the pointer to NULL to avoid multiple free of a same block.
    * 
    * @param pptr The address of the pointer allocated by SA_malloc, SA_calloc or SA_realloc
    */
    static inline void _SA_free_null(void** pptr)
    {
        _SA_free(*pptr);
        *pptr = NULL;
    }

    /**
    * @brief Free the memory block and set the pointer to NULL to avoid multiple free of a same block.
    * 
    * @param pptr The address of the pointer allocated by SA_malloc, SA_calloc or SA_realloc
    */
    #define SA_free(pptr) _SA_free_null((void**)(pptr))
#endif