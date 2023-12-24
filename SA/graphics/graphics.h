#ifndef SA_GRAPHICS
    #define SA_GRAPHICS

    #include "SA/global/global.h"

    typedef struct _SA_graphics_window SA_GraphicsWindow;

    enum SA_GRAPHICS_WINDOW_FLAGS {
        WINDOW_NORESIZE = 1 << 0,
    };

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window));
    SA_bool SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t color);
    SA_bool SA_graphics_vram_draw_horizontal_line(SA_GraphicsWindow* window, uint32_t x1, uint32_t x2, uint32_t y, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_vertical_line(SA_GraphicsWindow* window, uint32_t x, uint32_t y1, uint32_t y2, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_line(SA_GraphicsWindow* window, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint16_t thickness);
    
    void SA_graphics_vram_flush(SA_GraphicsWindow* window);
    
    #ifdef __cplusplus
    }
    #endif
#endif