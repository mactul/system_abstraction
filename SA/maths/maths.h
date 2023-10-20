#ifndef SA_MATHS
    #define SA_MATHS
    #include "SA/global/global.h"

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

    #ifdef __cplusplus
    }
    #endif
#endif