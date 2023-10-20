#ifndef SA_PATH
    #define SA_PATH
    #include "SA/global/global.h"

    #ifdef __cplusplus
    extern "C"{
    #endif
    
    void SA_simplify_path(char* dest, const char* src);

    void SA_path_join(char* path_dest, int nb_elements, ...);
    
    #ifdef __cplusplus
    }
    #endif
#endif