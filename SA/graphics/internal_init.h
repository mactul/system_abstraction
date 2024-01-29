#ifndef SA_GRAPHICS_INIT
    #define SA_GRAPHICS_INIT

    #include "SA/global/global.h"
    
    
    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Initialize the graphics protocol
     */
    void _SA_graphics_init(void);

    /**
     * @brief Stop the graphics protocol
     */
    void _SA_graphics_destroy(void);
    
    #ifdef __cplusplus
    }
    #endif
#endif