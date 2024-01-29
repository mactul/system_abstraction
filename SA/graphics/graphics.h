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

    /**
     * @brief Create and show a graphics window on screen
     * 
     * @param title Title of the window
     * @param pos_x Horizontal position of this window
     * @param pos_y Vertical position of this window
     * @param width Width of this window
     * @param height Height of this window
     * @param flags Window properties
     * @param draw_callback Function to handle selected events synchronously
     * @param events_to_queue Selected events for the synchronous callback
     * @param event_callback Function to handle all events asynchronously
     * @note `draw_callback` and `event_callback` are mutually exclusive
     */
    void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window), uint32_t events_to_queue, void (*event_callback)(SA_GraphicsWindow* window, SA_GraphicsEvent* event));
    
    /**
     * @brief Post an event to a window's event queue
     * 
     * @param window Pointer to the SA_GraphicsWindow structure related to this window
     * @param event Pointer to the SA_GraphicsEvent structure containing the event's data
     */
    void _SA_graphics_post_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);

    /**
     * @brief Poll for an event in a window's event queue
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param event Pointer to the SA_GraphicsEvent structure that will be filled with this function
     * @return - SA_FALSE if there is no event to handle
     * @return - SA_TRUE if there is an event to handle
     */
    SA_bool SA_graphics_poll_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);

    /**
     * @brief Poll for an event in a window's event queue
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param event Pointer to the SA_GraphicsEvent structure that will be filled with this function
     * @return - SA_FALSE if there is no event to handle
     * @return - SA_TRUE if there is an event to handle
     */
    SA_bool SA_graphics_wait_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event);
    

    /**
     * @brief Draw a pixel in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x Horizontal position of this pixel
     * @param y Vertical position of this pixel
     * @param color Color of this pixel in 24-bit RGB format
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the pixel exceeds the window bounds
     */
    SA_bool SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t color);

    /**
     * @brief Draw a horizontal line in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x1 Horizontal position of the first end of this line
     * @param x2 Horizontal position of the second end of this line
     * @param y Vertical position of this line
     * @param color Color of this line in 24-bit RGB format
     * @param thickness Thickness of this line (in pixels)
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the line exceeds the window bounds
     */
    SA_bool SA_graphics_vram_draw_horizontal_line(SA_GraphicsWindow* window, uint32_t x1, uint32_t x2, uint32_t y, uint32_t color, uint16_t thickness);

    /**
     * @brief Draw a horizontal line in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x Horizontal position of this line
     * @param y1 Vertical position of the first end of this line
     * @param y2 Vertical position of the second end of this line
     * @param color Color of this line in 24-bit RGB format
     * @param thickness Thickness of this line (in pixels)
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the line exceeds the window bounds
     */
    SA_bool SA_graphics_vram_draw_vertical_line(SA_GraphicsWindow* window, uint32_t x, uint32_t y1, uint32_t y2, uint32_t color, uint16_t thickness);

    /**
     * @brief Draw a horizontal line in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x1 Horizontal position of the first end of this line
     * @param y1 Vertical position of the first end of this line
     * @param x2 Horizontal position of the second end of this line
     * @param y2 Vertical position of the second end of this line
     * @param color Color of this line in 24-bit RGB format
     * @param thickness Thickness of this line (in pixels)
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the line exceeds the window bounds
     */
    SA_bool SA_graphics_vram_draw_line(SA_GraphicsWindow* window, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint16_t thickness);

    /**
     * @brief Draw a filled rectangle in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x Horizontal position of this rectangle
     * @param y Vertical position of this rectangle
     * @param width Width of this rectangle
     * @param height Height of this rectangle
     * @param color Color of this rectangle in 24-bit RGB format
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the rectangle exceeds the window bounds
    */
    SA_bool SA_graphics_vram_draw_rectangle(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);

    /**
     * @brief Draw a hollow rectangle in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x Horizontal position of this rectangle
     * @param y Vertical position of this rectangle
     * @param width Width of this rectangle
     * @param height Height of this rectangle
     * @param color Color of this rectangle in 24-bit RGB format
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the rectangle exceeds the window bounds
    */
    SA_bool SA_graphics_vram_draw_hollow_rectangle(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, uint32_t line_thickness);


    /**
     * @brief Draw a string in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x Horizontal position of the text
     * @param y Vertical position of the text
     * @param str Text to draw in the window
     * @param color Color of this text in 24-bit RGB format
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the text exceeds the window bounds
     */
    SA_bool SA_graphics_vram_draw_text(SA_GraphicsWindow* window, uint32_t x, uint32_t y, const char* str, uint32_t color);


    /**
     * @brief Flush all of a window's draw operations to show on screen
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     */
    void SA_graphics_vram_flush(SA_GraphicsWindow* window);


    /**
     * @brief Create a text input field in a graphical window's video ram
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param x Horizontal position of the text input
     * @param y Vertical position of the text input
     * @param background_color Background color of this text input in 24-bit RGB format
     * @param text_color Foreground color of this text input in 24-bit RGB format
     * @param max_text_size Maximum character count of this text input
     * @param padding_x Horizontal padding of the text input
     * @param padding_y Vertical padding of the text input
     * @return Pointer to a SA_GraphicsTextInput structure linked to this text input field
     */
    SA_GraphicsTextInput* SA_graphics_create_text_input(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t background_color, uint32_t text_color, uint32_t max_text_size, uint32_t padding_x, uint32_t padding_y);

    /**
     * @brief Re-draw a text input field in a graphical window's video ram (with its text value)
     * 
     * @param text_input Pointer to a SA_GraphicsTextInput structure linked to this text input field
     */
    void SA_graphics_redraw_text_input(const SA_GraphicsTextInput* text_input);

    /**
     * @brief Handle an event happening in a text input field
     * 
     * @param text_input Pointer to a SA_GraphicsTextInput structure linked to this text input field
     * @param event Pointer to the SA_GraphicsEvent structure containing the event's data
     * @return - SA_TRUE if the text input has focus
     * @return - SA_FALSE if the text input doesn't have focus
     */
    SA_bool SA_graphics_handle_text_input_events(SA_GraphicsTextInput* text_input, const SA_GraphicsEvent* event);

    /**
     * @brief Get the current value of a text input field
     * 
     * @param text_input Pointer to a SA_GraphicsTextInput structure linked to this text input field
     * @return String value of the text input field
     */
    const char* SA_graphics_get_text_input_value(SA_GraphicsTextInput* text_input);

    /**
     * @brief Get a text input field's dimension as a SA_GraphicsRectangle structure
     * 
     * @param text_input Pointer to a SA_GraphicsTextInput structure linked to this text input field
     * @param rectangle Pointer to a SA_GraphicsRectangle structure that will be filled with this function
     */
    void SA_graphics_get_text_input_rectangle(SA_GraphicsTextInput* text_input, SA_GraphicsRectangle* rectangle);

    /**
     * @brief Free a text input field's data
     * 
     * @param text_input Pointer to a pointer to a SA_GraphicsTextInput structure linked to this text input field
     */
    void SA_graphics_free_text_input(SA_GraphicsTextInput** text_input);

    /**
     * @brief Draw a filled rectangle in a graphical window's video ram from a SA_GraphicsRectangle structure
     * 
     * @param window Pointer to a SA_GraphicsWindow structure related to this window
     * @param rectangle Pointer to a SA_GraphicsRectangle structure related to this rectangle
     * @param color Color of this rectangle in 24-bit RGB format
     * @return - SA_TRUE if the operation succeeded
     * @return - SA_FALSE if the window was already closed or the rectangle exceeds the window bounds
    */
    static inline SA_bool SA_graphics_vram_draw_rectangle_from_rectangle(SA_GraphicsWindow* window, SA_GraphicsRectangle* rectangle, uint32_t color)
    {
        return SA_graphics_vram_draw_rectangle(window, rectangle->top_left_corner_x, rectangle->top_left_corner_y, rectangle->width, rectangle->height, color);
    }
    
    #ifdef __cplusplus
    }
    #endif
#endif