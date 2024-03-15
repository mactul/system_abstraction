#ifndef SA_GLOBAL
    #define SA_GLOBAL

    #include <stdint.h>
    #include <stddef.h>
    #include <sys/types.h>
    #include "SA/global/errors.h"

    #define SA_TRUE 1
    #define SA_FALSE 0

    typedef char SA_bool;
    typedef uint8_t SA_byte;

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
        // write `type SA_WARN_UNUSED_RESULT function(...) {` and function will raise a warning if his return value is unused.
        #define SA_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

        // write this instead of `break` in a switch statement to specifically say that you want the statement to falltrough.
        #define SA_FALLTROUGH __attribute__((falltrough))

        // cross platform version of [gnu pure attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-pure-function-attribute)
        #define SA_FUNCTION_PURE __attribute__((pure))

        // cross platform version of [gnu const attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-const-function-attribute)
        #define SA_FUNCTION_CONST __attribute__((const))

        // cross platform version of [gnu deprecated attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-deprecated-function-attribute)
        #define SA_FUNCTION_DEPRECATED __attribute__((deprecated))

        #define SA_FUNCTION_NOTHROW __attribute__((__nothrow__, __leaf__))

        // cross platform version of [gnu constructor attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-constructor-function-attribute)
        #define SA_FUNCTION_CONSTRUCTOR __attribute__((constructor))

        // cross platform version of [gnu destructor attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-destructor-function-attribute)
        #define SA_FUNCTION_DESTRUCTOR __attribute__((destructor))

        // cross platform version of [gnu malloc attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute)
        #if defined(__clang__)
            #define SA_MALLOC_FUNC(free_func) __attribute__((malloc))
        #else
            #define SA_MALLOC_FUNC(free_func) __attribute__((malloc, malloc(free_func, 1)))
        #endif
        

        // cross platform version of [gnu expect builtin](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html#index-_005f_005fbuiltin_005fexpect)
        #define SA_UNLIKELY(x) __builtin_expect(x, 0L)

        // cross platform version of [gnu expect builtin](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html#index-_005f_005fbuiltin_005fexpect)
        #define SA_LIKELY(x) __builtin_expect(!!(x), 1L)
    #else
        // This statement has no effect with your compiler
        #define SA_WARN_UNUSED_RESULT

        // This statement has no effect with your compiler
        #define SA_FALLTROUGH

        // This statement has no effect with your compiler
        #define SA_FUNCTION_PURE

        // This statement has no effect with your compiler
        #define SA_FUNCTION_CONST

        // This statement has no effect with your compiler
        #define SA_FUNCTION_NOTHROW

        // This statement has no effect with your compiler
        #define SA_MALLOC_FUNC(free_func)

        // This statement has no effect with your compiler
        #define SA_UNLIKELY(x) (x)

        // This statement has no effect with your compiler
        #define SA_LIKELY(x) (x)
    #endif

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief This function MUST BE put at the top of the main function before doing anything else with the library.
     * 
     * @note This function can be called multiple times without any issue.
    */
    void SA_init(void);

    /**
     * @brief This function must be put at the end of the program, after having closed all the threads.  
     * @brief In debug mode, it can display warning messages on stderr.
     * 
     * @note This function can be called multiple times without any issue.
    */
    void SA_destroy(void);
    
    #ifdef __cplusplus
    }
    #endif
#endif