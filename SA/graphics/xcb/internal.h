#ifndef SA_GRAPHICS_XCB_INTERNAL
    #define SA_GRAPHICS_XCB_INTERNAL

    #include "SA/global/global.h"

    #include <xcb/xcb.h>
    #include <pthread.h>

    struct _SA_graphics_window {
        xcb_connection_t* connection;
        xcb_window_t window;
        pthread_mutex_t mutex;
        int width;
        int height;
        SA_bool is_killed;
    };
#endif