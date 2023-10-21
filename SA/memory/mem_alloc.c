#include <malloc.h>
#include "SA/memory/mem_alloc.h"

#ifdef SA_MEMORY_DEBUG
    static uint64_t _allocations = 0;
#endif

/*
Allocate SIZE bytes of memory
*/
void* SA_malloc(uint64_t size)
{
    void* ptr = malloc(size);
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
void* SA_calloc(uint64_t size)
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
void* SA_realloc (void* ptr, uint64_t size)
{
    return realloc(ptr, size);
}


/*
Takes the adress of the pointer allocated by SA_malloc, SA_calloc or SA_realloc
free the memory block and set the pointer to NULL to avoid multiple free of a same block.
*/
void _SA_free(void** pptr)
{
    #ifdef SA_MEMORY_DEBUG
        if(*pptr != NULL)
        {
            _allocations--;
        }
    #endif
    free(*pptr);
    *pptr = NULL;
}

#ifdef SA_MEMORY_DEBUG
    /*
    Internal function only avaible in debug mode
    */
    SA_bool _SA_is_everything_freed(void)
    {
        return _allocations == 0;
    }
#endif