#include <stdlib.h>
#include "SA/memory/mem_alloc.h"

#ifdef SA_MEMORY_DEBUG
    static size_t _allocations = 0;
#endif

/*
Allocate SIZE bytes of memory
*/
void* SA_malloc(size_t size)
{
    void* ptr = malloc(size);
    #ifdef DEBUG
        if(SA_UNLIKELY(!_SA_is_init))
        {
            SA_print_error("DebugWarning: SA was never initialized\n");
            _SA_is_init = SA_TRUE;
        }
    #endif
    #ifdef SA_MEMORY_DEBUG
        if(ptr != NULL)
        {
            _allocations++;
        }
    #endif
    return ptr;
}

/*
Allocate SIZE bytes of memory and set them to 0
*/
void* SA_calloc(size_t size)
{
    void* ptr = calloc(size, 1);
    #ifdef SA_MEMORY_DEBUG
        if(ptr != NULL)
        {
            _allocations++;
        }
    #endif
    return ptr;
}


/*
Add SIZE bytes of memory in the previously allocated ptr block
*/
void* SA_realloc(void* ptr, size_t size)
{
    return realloc(ptr, size);
}


/*
Takes the address of the pointer allocated by SA_malloc, SA_calloc or SA_realloc
free the memory block and set the pointer to NULL to avoid multiple free of a same block.
*/
void _SA_free(void* ptr)
{
    #ifdef SA_MEMORY_DEBUG
        if(ptr != NULL)
        {
            _allocations--;
        }
    #endif
    free(ptr);
}

#ifdef SA_MEMORY_DEBUG
    /*
    Internal function only available in debug mode
    */
    SA_bool _SA_is_everything_freed(void)
    {
        return _allocations == 0;
    }
#endif