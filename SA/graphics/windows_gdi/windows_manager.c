#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <stdio.h>
#include <assert.h>
#include "SA/graphics/windows_gdi/internal.h"
#include "SA/graphics/graphics.h"
#include "SA/strings/strings.h"

#define MAX_255(x) x > 255 ? 255 : x
#define RELU(x) x < 0 ? 0 : x
#define RANGE_0_255(x) RELU(MAX_255(x))


typedef struct _thread_data {
    SA_GraphicsWindow* window;
    void (*draw_callback)(SA_GraphicsWindow* window);
} ThreadData;

static void change_bitmap_size(SA_GraphicsWindow* window, int new_width, int new_height)
{
    pthread_mutex_lock(&(window->mutex));

    if(new_height <= window->height && new_width <= window->width)
    {
        goto LABEL_END;
    }
    HBITMAP new_bitmap;
    HDC new_vram;
    new_vram = CreateCompatibleDC(window->hDC);
    new_bitmap = CreateCompatibleBitmap(window->hDC, new_width, new_height);
    SelectObject(new_vram, new_bitmap);

    BitBlt(new_vram, 0, 0, min(new_width, window->width), min(new_height, window->height), window->vram, 0, 0, SRCCOPY);
    
    DeleteObject(window->_bitmap_vram);
    DeleteDC(window->vram);

    window->_bitmap_vram = new_bitmap;
    window->vram = new_vram;

LABEL_END:

    window->width = new_width;
    window->height = new_height;

    pthread_mutex_unlock(&(window->mutex));
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_SIZE:
            SA_GraphicsWindow* window = GetWindowLongPtr(hwnd, GWLP_USERDATA);
            RECT window_area;
            GetClientRect(hwnd, &window_area);

            change_bitmap_size(window, window_area.right - window_area.left, window_area.bottom - window_area.top);
            printf("%d %d\n", window->width, window->height);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

static void* callback_runner(void* data)
{
    ThreadData* thread_data = (ThreadData*)data;

    thread_data->draw_callback(thread_data->window);

    thread_data->window->is_killed = SA_TRUE;
    return NULL;
}

void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window))
{
    SA_GraphicsWindow window = {.is_killed = SA_FALSE, .width = width, .height = height, .mutex = PTHREAD_MUTEX_INITIALIZER};

    HINSTANCE hThisInstance = GetModuleHandle(NULL);
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    char className[32];

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = SA_uint64_to_str(className, (uint64_t)(&window));
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    /* It seems that if we set this to NULL, windows doesn't refresh his backbuffer when the application is minified */
    wincl.hbrBackground = (HBRUSH) 0;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return;

    /* The class is registered, let's create the program*/
    window.hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           className,         /* Classname */
           title,       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           pos_x,       /* Windows decides the position */
           pos_y,       /* where the window ends up on the screen */
           width,                 /* The programs width */
           height,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    
    SetWindowLongPtr(window.hwnd, GWLP_USERDATA, (LONG_PTR)(&window));
    
    /* Make the window visible on the screen */
    ShowWindow(window.hwnd, SW_SHOWDEFAULT);
    
    window.hDC = GetDC(window.hwnd);
    window.vram = CreateCompatibleDC(window.hDC);
    window._bitmap_vram = CreateCompatibleBitmap(window.hDC, width, height);
    SelectObject(window.vram, window._bitmap_vram);

    window.mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_t thread;
    ThreadData thread_data = {.window = &window, .draw_callback = draw_callback};

    pthread_create(&thread, NULL, callback_runner, &thread_data);

    SetTimer(window.hwnd, 1, 100, NULL); 

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (!window.is_killed && GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);

        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    pthread_mutex_lock(&(window.mutex));
    DestroyWindow(window.hwnd);
    DeleteDC(window.hDC);
    DeleteDC(window.vram);
    DeleteObject(window._bitmap_vram);
    window.is_killed = SA_TRUE;
    window.hwnd = NULL;
    window.hDC = NULL;
    window._bitmap_vram = NULL;
    window.vram = NULL;
    window.width = 0;
    window.height = 0;
    pthread_mutex_unlock(&(window.mutex));

    pthread_join(thread, NULL);
}