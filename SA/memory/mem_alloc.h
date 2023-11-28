#ifndef SA_MEMORY
    #define SA_MEMORY
    #include "SA/global/global.h"

    #ifdef __cplusplus
    extern "C"{
    #endif

    void* SA_WARN_UNUSED_RESULT SA_malloc(uint64_t size);
    void* SA_WARN_UNUSED_RESULT SA_calloc(uint64_t size);
    void* SA_WARN_UNUSED_RESULT SA_realloc(void* ptr, uint64_t size);

    void _SA_free(void** pptr);
    
    /*
    Takes the adress of the pointer allocated by SA_malloc, SA_calloc or SA_realloc
    free the memory block and set the pointer to NULL to avoid multiple free of a same block.
    */
    #define SA_free(pptr) _SA_free((void**)(pptr))

    #ifdef SA_MEMORY_DEBUG
        SA_bool _SA_is_everything_freed(void);
    #endif

    #ifdef __cplusplus
    }
    #endif
#endif