#ifndef SA_GRAPHICS_X11_INTERNAL
    #define SA_GRAPHICS_X11_INTERNAL

    #include "SA/global/global.h"

    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <pthread.h>

    struct _SA_graphics_window {
        Display* display;
        Window window;
        GC gc;
        Drawable vram;
        Drawable renderer;  // This backbuffer is needed to handle the "Expose" event
        pthread_mutex_t mutex;
        int width;
        int height;
        SA_bool is_killed;
    };
#endif