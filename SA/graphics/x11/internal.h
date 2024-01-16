#ifndef SA_GRAPHICS_X11_INTERNAL
    #define SA_GRAPHICS_X11_INTERNAL

    #include "SA/global/global.h"
    #include "SA/memory/queue.h"

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
        uint32_t events_to_queue;
        SA_Queue* event_queue;
        void (*event_callback)(SA_GraphicsWindow* window, SA_GraphicsEvent* event);
        int width;
        int height;
        SA_bool is_killed;
    };
#endif