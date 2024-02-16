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

    /**
     * @brief Generate an uint64_t integer between the range 0 and SA_RAND_U64_MAX.  
     * @brief This is extremly fast, but unsafe for cryptographic use.
     * @brief It's based on xorshift128+.
     * 
     * @return uint64_t 
     * 
     * @note NOT SAFE for cryptographic use
     */
    uint64_t SA_random_unsecure_uint64(void);

    /**
     * @brief Set the random seed for all unsecure random functions.  
     * @brief The seed is automaticaly set by SA_init() but you can reset it to a fixed value if you want to always have the same results in a simulation for example.
     * 
     * @param seed a arbitrary number for seeding the algorithm.
     * 
     * @note - This seed will be transformed before being given to xorshift128+, so you can pass 0 or whatever you want.
     * @note - Setting the seed is not as fast as choosing a random number, you should just set the seed once
     */
    void SA_set_unsecure_seed(uint64_t seed);

    /**
     * @brief Generate an int64_t integer between the range start and end.  
     * @brief This is extremly fast, but unsafe for cryptographic use.
     * @brief It's based on xorshift128+.
     * 
     * @return uint64_t 
     * 
     * @note NOT SAFE for cryptographic use
     */
    int64_t SA_random_unsecure_int64(int64_t start, int64_t end);

    /**
     * @brief Generate an double between the range start and end.  
     * @brief This is extremly fast, but unsafe for cryptographic use.
     * @brief It's based on xorshift128+.
     * 
     * @return double 
     * 
     * @note NOT SAFE for cryptographic use
     */
    double SA_random_unsecure_float(double start, double end);

    /**
     * @brief Generate a seed with more and more entropy at each call
     * 
     * @return SA_random_seed, whether a __uint128_t or a uint64_t depending on the system
     */
    SA_random_seed SA_random_standard_seed(void);

    /**
     * @brief The blum blum shub algorithm.  
     * @brief Just like this, this implementation isn't totaly safe, because M isn't big enough. But coupled with SA_random_standard_seed, it's extremly safe.
     * @brief You should prefere to not use the function directly
     * 
     * @param n the number of iterations the algorithm should do, determines the number of bytes of the result
     * @param seed the seed in which you want to apply bbs (you should use `SA_random_standard_seed`)
     * @return an uint64_t with the first N bytes random
     */
    uint64_t SA_random_bbs(int n, SA_random_seed seed);

    /**
     * @brief Generate a random integer between `start` and `end`.  
     * @brief This function is safe for cryptographic applications.
     * @brief For simulations purpose, you should use the unsecure version which is much quicker.
     * 
     * @return int64_t
     */
    int64_t SA_random_secured_int64(int64_t start, int64_t end);

    /**
     * @brief Generate a random double between `start` and `end`.  
     * @brief This function is safe for cryptographic applications.
     * @brief For simulations purpose, you should use the unsecure version which is much quicker.
     * 
     * @return double
     */
    double SA_random_secured_float(double start, double end);

    /**
     * @brief Generate a very safe random base64 string
     * 
     * @param result the buffer to store the random string. should be at least the size of len+1.
     * @param len the length of the random string to generate.
     */
    void SA_random_secure_base64_string(char* result, int len);

    /**
     * @brief Generate `number_of_bytes` random bytes  
     * 
     * @note This function is safe for cryptographic applications
     * 
     */
    void SA_random_secure_bytes(void* bytes, size_t number_of_bytes);

    #ifdef __cplusplus
    }
    #endif
#endif