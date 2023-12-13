#ifndef SA_GRAPHICS
    #define SA_GRAPHICS

    #include "SA/global/global.h"

    typedef struct _SA_graphics_window SA_GraphicsWindow;

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window));
    void SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, int x, int y, uint32_t color);
    void SA_graphics_vram_flush(SA_GraphicsWindow* window);
    
    #ifdef __cplusplus
    }
    #endif
#endif