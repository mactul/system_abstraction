#ifndef SA_GRAPHICS_WINDOWS_GDI_INTERNAL
    #define SA_GRAPHICS_WINDOWS_GDI_INTERNAL

    #include <pthread.h>
    #include <windows.h>
    #include "SA/global/global.h"
    #include "SA/memory/queue.h"

    struct _SA_graphics_window {
        HWND hwnd;
        HDC hDC;
        HDC vram;
        HBITMAP _bitmap_vram;
        SA_Queue* event_queue;
        pthread_mutex_t mutex;
        uint32_t width;
        uint32_t height;
        uint32_t events_to_handle;
        SA_bool is_killed;
    };
#endif