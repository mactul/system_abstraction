#include <assert.h>
#include "SA/random/random.h"
#include "SA/time/time.h"

static uint64_t _unsecure_seed = 1;


/*
Set the random seed for all unsecure random functions.
The seed is automaticly set by SA_init() but you can reset it to a fixed value if you want to always have the same results in a simulation for example.
*/
void SA_set_unsecure_seed(uint64_t seed)
{
	_unsecure_seed = seed;
}


/*
Generate an uint64_t integer between the range 0 and SA_RAND_U64_MAX
*/
uint64_t SA_random_unsecure_uint64(void)
{
    #ifdef DEBUG
        if(!_SA_is_init)
        {
            SA_print_error("DebugWarning: SA was never initialized\n");
            _SA_is_init = SA_TRUE;
        }
    #endif
	return ((_unsecure_seed = SA_time_seed_ns() + _unsecure_seed * 1103515245ULL + 12345ULL) % SA_RAND_U64_MAX);
}


/*
Generate a fast random integer in the range given.
This function is NOT safe for cryptographic applications.
*/
int64_t SA_random_unsecure_int64(int64_t start, int64_t end)
{
    assert(start <= end);
    
    return start + (int64_t)(SA_random_unsecure_uint64() % (uint64_t)(end - start + 1));
}

/*
Generate a fast random float in the range given.
This function is NOT safe for cryptographic applications.
*/
double SA_random_unsecure_float(double start, double end)
{
    assert(start <= end);
    double total = (double)(SA_RAND_U64_MAX) / (end - start);
    return start + (double)SA_random_unsecure_uint64() / total;
}