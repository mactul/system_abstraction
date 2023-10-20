#ifndef SA_GLOBAL
    #define SA_GLOBAL

    #include <stdint.h>
    #include "SA/global/errors.h"

    /*
    Set here the flags you want to use in debug mode
    */
    #ifdef DEBUG
        #define SA_MEMORY_DEBUG
    #endif

    #if defined(_WIN32) || defined(WIN32)
        #define SA_OS_IS_WINDOWS
    #else
        #define SA_OS_IS_UNIX
    #endif

    #define SA_TRUE 1
    #define SA_FALSE 0

    #define NULL (void*)(0)

    typedef char SA_bool;

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_init(void);
    void SA_destroy(void);
    
    #ifdef __cplusplus
    }
    #endif
#endif