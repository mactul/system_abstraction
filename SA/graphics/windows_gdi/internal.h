#ifndef SA_GRAPHICS_WINDOWS_GDI_INTERNAL
    #define SA_GRAPHICS_WINDOWS_GDI_INTERNAL

    #include "SA/global/global.h"

    #include <pthread.h>
    #include <windows.h>

    struct _SA_graphics_window {
        HWND hwnd;
        HDC hDC;
        HDC vram;
        HBITMAP _bitmap_vram;
        pthread_mutex_t mutex;
        int width;
        int height;
        SA_bool is_killed;
    };
#endif