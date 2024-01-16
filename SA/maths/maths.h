#ifndef SA_MATHS
    #define SA_MATHS
    #include "SA/global/global.h"

    /*
    Take the maximum value between x and y
    /!\\ WARNING ! /!\\ One parameter will be evaluated twice.
    */
    #define SA_MAX(x, y) ((x) > (y) ? (x): (y))

    /*
    Take the minimum value between x and y
    /!\\ WARNING ! /!\\ One parameter will be evaluated twice.
    */
    #define SA_MIN(x, y) ((x) < (y) ? (x): (y))

    /*
    Warning, the expression entered will be evaluated twice by the macro.
    Returns 0 if x is negative, x otherwise.
    ```
    ........|....../.
    ........|..../...
    ........|../.....
    .=======|/------.
    ........|........
    ........|........
    ........|........
    ```
    */
    #define SA_RELU(x) ((x) < 0 ? 0 : (x))

    #define SA_sigmoid(x) SA_logistic(x)

    #ifdef __cplusplus
    extern "C"{
    #endif

    uint64_t SA_int_pow(uint32_t base, unsigned char e);
    double SA_logistic(double x);

    SA_bool SA_float_equals(double a, double b);

    #ifdef __cplusplus
    }
    #endif
#endif