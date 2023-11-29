#ifndef SA_DIRECTORIES
    #define SA_DIRECTORIES
    #include "SA/global/global.h"

    #ifdef __cplusplus
    extern "C"{
    #endif
    
    void SA_recursive_mkdir(const char* path);
    
    #ifdef __cplusplus
    }
    #endif
#endif