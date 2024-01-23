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

    /**
    * @brief Returns 0 if x is negative, x otherwise.
    * @brief ```
    * @brief Warning, the expression entered will be evaluated twice by the macro.  
    * @brief ........|....../.
    * @brief ........|..../...
    * @brief ........|../.....
    * @brief .=======|/------.
    * @brief ........|........
    * @brief ........|........
    * @brief ........|........
    * @brief ```
    */
    #define SA_RELU(x) ((x) < 0 ? 0 : (x))

    // Alias for the logistic function
    #define SA_sigmoid(x) SA_logistic(x)

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Compute base^e in the fastest way for integers
     * @param base
     * @param e
     * @return pow(base, e)
    */
    uint64_t SA_FUNCTION_CONST SA_int_pow(uint32_t base, unsigned char e);

    /**
     * @brief Returns the logistic (sometimes called sigmoid) function  
     * @brief 1/(1+e^(-x))
     * @brief ```
     * @brief ............|........===. 1
     * @brief ............|..../.......
     * @brief ............|./..........1 /
     * @brief ........../.|............ / 2
     * @brief ......./....|............
     * @brief .===________|___________. 0
     * @brief ............|............
     * @brief ............|............
     * @brief ............|............
     * @brief ```
     * @param x the real to evaluate
     * @return 1/(1+e^(-x))
    */
    double SA_FUNCTION_CONST SA_logistic(double x);

    /**
     * @brief Check if 2 floats are equals +/- 1e-8
     * @param a 
     * @param b 
     * @return a == (b +/- 1e-8)
    */
    SA_bool SA_FUNCTION_CONST SA_float_equals(double a, double b);

    #ifdef __cplusplusthe parameter
    }
    #endif
#endif