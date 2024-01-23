#include <math.h>
#include "SA/maths/maths.h"

#define SA_EPSILON 1e-8

/*
Compute base^e.
*/
uint64_t SA_int_pow(uint32_t base, unsigned char e)
{
    uint64_t result = 1;
    for (;;)
    {
        if(e & 1)
            result *= base;
        e >>= 1;
        if(e == 0)
            break;  // here to avoid a non usefull multiplication
        base *= base;
    }

    return result;
}

/*
Returns the logistic (sometimes called sigmoid) function
1/(1+e^(-x))
```
............|........===. 1
............|..../.......
............|./..........1 /
........../.|............ / 2
......./....|............
.===________|___________. 0
............|............
............|............
............|............
```
*/
double SA_logistic(double x)
{
    return 0.5 + 0.5*tanh(x/2.0);
}

SA_bool SA_float_equals(double a, double b)
{
    return fabs(a - b) < SA_EPSILON;
}