#include <assert.h>
#include "SA/random/random.h"
#include "SA/time/time.h"

// P1 and P2 are large prime numbers
#ifdef __SIZEOF_INT128__
    #define P1 (SA_random_seed)4294955131
    #define P2 (SA_random_seed)3294955019
#else
    #define P1 61819ULL
    #define P2 56543ULL
#endif
#define M (P1*P2)

#define N 64

static SA_random_seed _secure_seed = 0;

/*
This function generated a seed for secure random generators.
At each call, the seed gain a little bit of entropy, which is amplified by the random algorithm.
*/
SA_random_seed SA_random_standard_seed(void)
{
    static SA_random_seed i = 0;

    // this will overflow, but it's not a problem for generating random numbers
    i += 1 + SA_time_seed_ns() + _secure_seed;

    // This weird code prevent the seed to being a multiple of M
    while(i == P1 || i == P2 || i == M || i == 0)
    {
        i++;
    }
    i = i % M;
    return i;
}

/*
This function is a small implementation of the Blum Blum Shub algorithm.
Just like this, this implementation isn't totaly safe, because M isn't big enough. But coupled with SA_random_standard_seed, it's extremly safe.
*/
uint64_t SA_random_bbs(int n, SA_random_seed seed)
{
    SA_random_seed a = 0;
    for(int i = 0; i < n; i++)
    {
        if((seed & (SA_random_seed)1) == (SA_random_seed)1)
            a += (1ULL << i);
        seed = (seed*seed) % M;
    }
    _secure_seed = seed;
    return a;
}

/*
Generate a random integer in the range given.
This function is safe for cryptographic applications.
For simulations purpose, you should use the unsecure version which is much quicker.
*/
int64_t SA_random_secured_int64(int64_t start, int64_t end)
{
    assert(start <= end);
    uint64_t result = SA_random_bbs(N, SA_random_standard_seed());
    
    return start + (result % (end - start + 1));
}

/*
Generate a random float in the range given.
This function is safe for cryptographic applications.
For simulations purpose, you should use the unsecure version which is much quicker.
*/
double SA_random_secured_float(double start, double end)
{
    assert(start <= end);
    double total = (double)(SA_RAND_U64_MAX) / (end - start);
    return start + (double)SA_random_bbs(N, SA_random_standard_seed()) / total;
}

static const char basis_64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/*
Generate a secure token of size LEN in RESULT with the base64 characters.
*/
void SA_random_secure_base64_string(char* result, int len)
{
    for(int i = 0; i < len; i++)
    {
        result[i] = basis_64[SA_random_secured_int64(0, sizeof(basis_64)-2)];
    }
    result[len] = '\0';
}