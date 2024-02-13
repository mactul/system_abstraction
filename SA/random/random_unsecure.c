#include <assert.h>
#include "SA/random/random.h"
#include "SA/time/time.h"

static struct xorshift128p_state {
    uint64_t x[2];
} _unsecure_seed = {.x = {0, 1}};


/*
Used to change a seed to a better one, that can't be 0.
*/
static uint64_t transform_seed(uint64_t n)
{
    n += 0x9E3779B97f4A7C15;
    n = (n ^ (n >> 30)) * 0xBF58476D1CE4E5B9;
    n = (n ^ (n >> 27)) * 0x94D049BB133111EB;
    n = n ^ (n >> 31);
    if(n == 0)
    {
        return 0x18A270F24D80907B;
    }
    return n;
}

/*
Set the random seed for all unsecure random functions.
The seed is automaticaly set by SA_init() but you can reset it to a fixed value if you want to always have the same results in a simulation for example.
*/
void SA_set_unsecure_seed(uint64_t seed)
{
    _unsecure_seed.x[0] = transform_seed(seed);
    _unsecure_seed.x[0] = transform_seed(seed + _unsecure_seed.x[0]);
}


/*
Generate an uint64_t integer between the range 0 and SA_RAND_U64_MAX using xorshift128+
*/
uint64_t SA_random_unsecure_uint64(void)
{
    #ifdef DEBUG
        if(SA_UNLIKELY(!_SA_is_init))
        {
            SA_print_error("DebugWarning: SA was never initialized\n");
            _SA_is_init = SA_TRUE;
        }
    #endif
    uint64_t t = _unsecure_seed.x[0];
    const uint64_t s = _unsecure_seed.x[1];
    _unsecure_seed.x[0] = s;
    t ^= t << 23;
    t ^= t >> 18;
    t ^= s ^ (s >> 5);
    _unsecure_seed.x[1] = t;
    return t + s;
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