#ifndef SA_MEMORY
    #define SA_MEMORY
    #include "SA/global/global.h"

    #ifdef __cplusplus
    extern "C"{
    #endif

    void* SA_malloc(uint64_t size);
    void* SA_calloc(uint64_t size);
    #ifdef __GNUC__
        void* __attribute__((warn_unused_result)) SA_realloc(void* ptr, uint64_t size);
    #else
        void* SA_realloc(void* ptr, uint64_t size);
    #endif

    void _SA_free(void** pptr);
    
    /*
    Takes the adress of the pointer allocated by SA_malloc, SA_calloc or SA_realloc
    free the memory block and set the pointer to NULL to avoid multiple free of a same block.
    */
    #define SA_free(pptr) _SA_free((void**)pptr)

    #ifdef SA_MEMORY_DEBUG
        SA_bool _SA_is_everything_freed(void);
    #endif

    #ifdef __cplusplus
    }
    #endif
#endif