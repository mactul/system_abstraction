#ifndef SA_GLOBAL
    #define SA_GLOBAL

    #include <stdint.h>
    #include <stddef.h>
    #include "SA/global/errors.h"

    #define SA_TRUE 1
    #define SA_FALSE 0

    typedef char SA_bool;
    typedef char byte;

    /*
    Set here the flags you want to use in debug mode
    */
    #ifdef DEBUG
        extern SA_bool _SA_is_init;
        #define SA_MEMORY_DEBUG
    #endif

    #if defined(_WIN32) || defined(WIN32)
        #define SA_OS_IS_WINDOWS
    #else
        #define SA_OS_IS_UNIX
    #endif

    #ifdef __GNUC__
        #define SA_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
    #else
        #define SA_WARN_UNUSED_RESULT
    #endif

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_init(void);
    void SA_destroy(void);
    
    #ifdef __cplusplus
    }
    #endif
#endif