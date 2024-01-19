#ifndef SA_GRAPHICS
    #define SA_GRAPHICS

    #include "SA/global/global.h"

    #define SA_GRAPHICS_EVENT_QUEUE_LENGTH 1024

    typedef struct _SA_graphics_window SA_GraphicsWindow;
    typedef struct _SA_graphics_text_input SA_GraphicsTextInput;

    enum SA_GRAPHICS_WINDOW_FLAGS {
        SA_GRAPHICS_WINDOW_RESIZE = 1 << 0,
    };

    enum SA_GRAPHICS_TO_QUEUE {
        SA_GRAPHICS_QUEUE_MOUSE_CLICK = 1 << 0,
        SA_GRAPHICS_QUEUE_KEYBOARD    = 1 << 1,
        SA_GRAPHICS_QUEUE_MOUSE_MOVE  = 1 << 2,
        SA_GRAPHICS_QUEUE_SCROLL      = 1 << 3,

        SA_GRAPHICS_QUEUE_EVERYTHING  = (uint32_t)(-1)
    };

    enum SA_GRAPHICS_EVENTS {
        SA_GRAPHICS_EVENT_NOTHING = -1,
        SA_GRAPHICS_EVENT_CLOSE_WINDOW = 0,
        SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN,
        SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP,
        SA_GRAPHICS_EVENT_MOUSE_MOVE,
        SA_GRAPHICS_EVENT_SCROLL_UP,
        SA_GRAPHICS_EVENT_SCROLL_DOWN,
        SA_GRAPHICS_EVENT_KEY_DOWN,
        SA_GRAPHICS_EVENT_KEY_UP,
    };

    enum SA_GRAPHICS_KEYCODES {
        SA_GRAPHICS_KEYCODE_BACKSPACE = 22,
        SA_GRAPHICS_KEYCODE_ENTER     = 36,
    };

    typedef struct _sa_graphics_rectangle {
        uint32_t top_left_corner_x;
        uint32_t top_left_corner_y;
        uint32_t width;
        uint32_t height;
    } SA_GraphicsRectangle;

    typedef struct _SA_event_mouse {
        uint32_t x;
        uint32_t y;
    } SA_EventMouse;

    typedef struct _SA_event_key {
        uint32_t keycode;
        char str[10];
    } SA_EventKey;

    typedef struct _sa_graphics_event {
        enum SA_GRAPHICS_EVENTS event_type;
        union events {
            SA_EventMouse mouse;
            SA_EventKey key;
        } events;
    } SA_GraphicsEvent;

    #ifdef __cplusplus
    extern "C"{
    #endif

    void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window), uint32_t events_to_queue, void (*event_callback)(SA_GraphicsWindow* window, SA_GraphicsEvent* event));
    
    SA_bool SA_graphics_poll_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);
    SA_bool SA_graphics_wait_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);
    
    SA_bool SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t color);
    SA_bool SA_graphics_vram_draw_horizontal_line(SA_GraphicsWindow* window, uint32_t x1, uint32_t x2, uint32_t y, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_vertical_line(SA_GraphicsWindow* window, uint32_t x, uint32_t y1, uint32_t y2, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_line(SA_GraphicsWindow* window, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint16_t thickness);
    SA_bool SA_graphics_vram_draw_rectangle(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
    SA_bool SA_graphics_vram_draw_hollow_rectangle(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, uint32_t line_thickness);

    SA_bool SA_graphics_vram_draw_text(SA_GraphicsWindow* window, uint32_t x, uint32_t y, const char* str, uint32_t color);

    void SA_graphics_vram_flush(SA_GraphicsWindow* window);

    SA_GraphicsTextInput* SA_graphics_create_text_input(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t background_color, uint32_t text_color, uint32_t max_text_size, uint32_t padding_x, uint32_t padding_y);
    void SA_graphics_redraw_text_input(const SA_GraphicsTextInput* text_input);
    SA_bool SA_graphics_handle_text_input_events(SA_GraphicsTextInput* text_input, const SA_GraphicsEvent* event);
    const char* SA_graphics_get_text_input_value(SA_GraphicsTextInput* text_input);
    void SA_graphics_get_text_input_rectangle(SA_GraphicsTextInput* text_input, SA_GraphicsRectangle* rectangle);
    void SA_graphics_free_text_input(SA_GraphicsTextInput** text_input);

    static inline SA_bool SA_graphics_vram_draw_rectangle_from_rectangle(SA_GraphicsWindow* window, SA_GraphicsRectangle* rectangle, uint32_t color)
    {
        return SA_graphics_vram_draw_rectangle(window, rectangle->top_left_corner_x, rectangle->top_left_corner_y, rectangle->width, rectangle->height, color);
    }
    
    #ifdef __cplusplus
    }
    #endif
#endif