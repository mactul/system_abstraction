#ifndef SA_TESTS
    #define SA_TESTS

    #include <assert.h>
    #include "SA/SA.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    void test_strings(void);
    void test_path(void);
    void test_parser(void);
    void test_math(void);
    void test_memory(void);
    void test_network(void);
    void test_graphics(void);

    #ifdef __cplusplus
    }
    #endif
#endif