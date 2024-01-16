#ifndef SA_GRAPHICS
    #define SA_GRAPHICS

    #include "SA/global/global.h"

    #define SA_GRAPHICS_EVENT_QUEUE_LENGTH 1024

    typedef struct _SA_graphics_window SA_GraphicsWindow;

    enum SA_GRAPHICS_WINDOW_FLAGS {
        SA_GRAPHICS_WINDOW_RESIZE = 1 << 0,
    };

    enum SA_GRAPHICS_TO_QUEUE {
        SA_GRAPHICS_QUEUE_MOUSE    = 1 << 0,
        SA_GRAPHICS_QUEUE_KEYBOARD = 1 << 1,


        SA_GRAPHICS_QUEUE_EVERYTHING = (uint32_t)(-1)
    };

    enum SA_GRAPHICS_EVENTS {
        SA_GRAPHICS_EVENT_NOTHING = -1,
        SA_GRAPHICS_EVENT_CLOSE_WINDOW = 0,
        SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN,
        SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP,
    };

    typedef struct _SA_event_click {
        uint32_t x;
        uint32_t y;
    } SA_EventClick;

    typedef struct _sa_graphics_event {
        enum SA_GRAPHICS_EVENTS event_type;
        union events {
            SA_EventClick click;
        } events;
    } SA_GraphicsEvent;

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window), uint32_t events_to_queue, void (*event_callback)(SA_GraphicsWindow* window, SA_GraphicsEvent* event));
    
    //void SA_graphics_post_event(SA_GraphicsEvent* event);
    SA_bool SA_graphics_poll_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);
    SA_bool SA_graphics_wait_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);
    
    SA_bool SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t color);
    SA_bool SA_graphics_vram_draw_horizontal_line(SA_GraphicsWindow* window, uint32_t x1, uint32_t x2, uint32_t y, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_vertical_line(SA_GraphicsWindow* window, uint32_t x, uint32_t y1, uint32_t y2, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_line(SA_GraphicsWindow* window, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint16_t thickness);
    
    void SA_graphics_vram_flush(SA_GraphicsWindow* window);
    
    #ifdef __cplusplus
    }
    #endif
#endif