#ifndef SA_TIME
    #define SA_TIME
    #include "SA/global/global.h"

    // Represents a number of seconds
    typedef uint64_t SA_seconds;

    // Represents a number of microseconds
    typedef uint64_t SA_microseconds;

    // Represents a number of nanoseconds
    typedef uint64_t SA_nanoseconds;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Returns the number of seconds since 01/01/1970 00h00
     * 
     * @return SA_seconds
     */
    SA_seconds SA_time(void);

    /**
     * @brief Returns the number of microseconds used by the CPU to run the program.
     * @brief This is perfect to test the performances of the program, since this is not influenced by sleep time and the CPU load taken by other programs on the machine.
     * 
     * @return SA_microseconds 
     */
    SA_microseconds SA_cpu_usage(void);

    /**
     * @brief Returns the number of nanoseconds since 01/01/1970 00h00  
     * @brief You can use this to make a very precise chrono
     * 
     * @return SA_nanoseconds 
     */
    SA_nanoseconds SA_time_ns(void);

    /**
     * @brief Returns the number of nanoseconds since the last second.
     * @brief This can be used to generate seeds.
     * 
     * @return SA_nanoseconds 
     */
    SA_nanoseconds SA_time_seed_ns(void);

    /**
     * @brief Sleep for `n` microseconds (+ the time taken by the function to be executed)
     * 
     * @param n the number of microseconds to sleep
     */
    void SA_sleep(SA_microseconds n);

    #ifdef __cplusplus
    }
    #endif


    /**
     * @brief Sleep for `seconds` seconds with a microseconds precision
     * 
     * @param seconds the number of seconds to sleep, can be less than 1
     */
    static inline void SA_sleep_seconds(double seconds)
    {
        SA_sleep(seconds * 1e6);
    }
#endif