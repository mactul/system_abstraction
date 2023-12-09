#ifndef SA_GRAPHICS
    #define SA_GRAPHICS

    #include "SA/global/global.h"

    typedef struct _SA_graphics_window SA_GraphicsWindow;

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags);
    
    #ifdef __cplusplus
    }
    #endif
#endif