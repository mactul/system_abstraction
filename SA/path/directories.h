#ifndef SA_DIRECTORIES
    #define SA_DIRECTORIES
    #include "SA/global/global.h"

    #ifdef __cplusplus
    extern "C"{
    #endif
    

    /**
     * @brief Create a directory with parents directories if needed
     * 
     * @param path The path of the directory to create
     * 
     * @note Despite his name, this function is non-recu
     */
    void SA_recursive_mkdir(const char* path);
    
    #ifdef __cplusplus
    }
    #endif
#endif