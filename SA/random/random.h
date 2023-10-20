#ifndef SA_RANDOM
    #define SA_RANDOM
    #include "SA/global/global.h"

    /*
    The maximum output number possible for u64 random functions
    */
    #define SA_RAND_U64_MAX (uint64_t)(-1)


    #ifdef __SIZEOF_INT128__
        // Here this is implemented as a 128 bits long integer, but this can be downgraded to a 64 bits on older systems.
        typedef __uint128_t SA_random_seed;
    #else
        // Here this is implemented as a 64 bits long integer, but this can be upgraded to a 128 bits on newer systems.
        typedef uint64_t SA_random_seed;
    #endif


    #ifdef __cplusplus
    extern "C"{
    #endif

    uint64_t SA_random_unsecure_uint64(void);
    void SA_set_unsecure_seed(uint64_t seed);
    int64_t SA_random_unsecure_int64(int64_t start, int64_t end);
    double SA_random_unsecure_float(double start, double end);

    SA_random_seed SA_random_standard_seed(void);
    uint64_t SA_random_bbs(int n, SA_random_seed seed);
    int64_t SA_random_secured_int64(int64_t start, int64_t end);
    void SA_random_secure_base64_string(char* result, int len);
    double SA_random_secured_float(double start, double end);

    #ifdef __cplusplus
    }
    #endif
#endif