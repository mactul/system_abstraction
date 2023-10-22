#ifndef SA_TIME
    #define SA_TIME
    #include "SA/global/global.h"

    typedef uint64_t SA_seconds;
    typedef uint64_t SA_microseconds;
    typedef uint64_t SA_nanoseconds;

    #ifdef __cplusplus
    extern "C"{
    #endif

    SA_seconds SA_time(void);
    SA_microseconds SA_cpu_usage(void);
    SA_nanoseconds SA_time_ns(void);
    SA_nanoseconds SA_time_seed_ns(void);
    void SA_sleep(SA_microseconds n);

    #ifdef __cplusplus
    }
    #endif
#endif