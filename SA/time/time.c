#include <time.h>
#include <unistd.h>
#include "SA/time/time.h"

/*
Returns the number of seconds since 01/01/1970
*/
SA_seconds SA_time(void)
{
    time_t t = time(NULL);

    return (uint64_t) difftime(t, 0);
}

/*
Returns the number of nanoseconds since 01/01/1970
*/
SA_nanoseconds SA_time_ns(void)
{
    struct timespec t;

    clock_gettime(CLOCK_REALTIME, &t);

    return 1000000000 * t.tv_sec + t.tv_nsec;
}

/*
Returns the number of nanoseconds since the last second.
This can be used to generate seeds.
*/
SA_nanoseconds SA_time_seed_ns(void)
{
    struct timespec t;

    clock_gettime(CLOCK_REALTIME, &t);

    return t.tv_nsec;
}

/*
Returns the number of microseconds used by the CPU to run the program.
This is perfect to test the performances of the program, since this is not influenced by sleep time and the CPU load taken by other programs on the machine.
*/
SA_microseconds SA_cpu_usage(void)
{
    clock_t ticks = clock();
    return (1000000 * ticks) / CLOCKS_PER_SEC;
}