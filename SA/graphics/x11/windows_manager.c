#include <sys/select.h>
#include <pthread.h>
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/graphics.h"
#include "SA/graphics/x11/internal.h"

#define min(a, b) ((a) < (b) ? (a): (b))

typedef struct _thread_data {
    SA_GraphicsWindow* window;
    void (*draw_callback)(SA_GraphicsWindow* window);
} ThreadData;

static void* callback_runner(void* data)
{
    ThreadData* thread_data = (ThreadData*)data;

    thread_data->draw_callback(thread_data->window);

    thread_data->window->is_killed = SA_TRUE;

    return NULL;
}


static int wait_fd(int fd, int ms)
{
    struct timeval tv;
    fd_set in_fds;
    FD_ZERO(&in_fds);
    FD_SET(fd, &in_fds);
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000)*1000;
    return select(fd+1, &in_fds, 0, 0, &tv);
}

SA_bool XNextEventTimeout(Display *display, XEvent *event, int ms)
{
    if (XPending(display) || wait_fd(ConnectionNumber(display), ms))
    {
        XNextEvent(display, event);
        return SA_TRUE;
    }
    return SA_FALSE;
}

static void change_bitmap_size(SA_GraphicsWindow* window, int new_width, int new_height)
{
    pthread_mutex_lock(&(window->mutex));

    if(new_height <= window->height && new_width <= window->width)
    {
        goto LABEL_END;
    }
    Drawable new_vram;
    Drawable new_renderer;
    new_vram = XCreatePixmap(window->display, window->window, new_width, new_height, 24);
    new_renderer = XCreatePixmap(window->display, window->window, new_width, new_height, 24);

    XCopyArea(window->display, window->vram, new_vram, window->gc, 0, 0, min(new_width, window->width), min(new_height, window->height), 0, 0);
    XCopyArea(window->display, window->renderer, new_renderer, window->gc, 0, 0, min(new_width, window->width), min(new_height, window->height), 0, 0);
    
    XFreePixmap(window->display, window->vram);
    XFreePixmap(window->display, window->renderer);

    window->renderer = new_renderer;
    window->vram = new_vram;

LABEL_END:

    window->width = new_width;
    window->height = new_height;

    pthread_mutex_unlock(&(window->mutex));
}

void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window), uint32_t events_to_queue, void (*event_callback)(SA_GraphicsWindow* window, SA_GraphicsEvent* event))
{
    SA_GraphicsWindow window = {.is_killed = SA_FALSE, .width = width, .height = height, .mutex = PTHREAD_MUTEX_INITIALIZER};

    pthread_t thread;
    ThreadData thread_data = {.window = &window, .draw_callback = draw_callback};

    window.display = XOpenDisplay(NULL);

    window.window = XCreateSimpleWindow(window.display, DefaultRootWindow(window.display), pos_x, pos_y, width, height, 0, 0x000000, 0xFF7000);
    if(!(flags & SA_GRAPHICS_WINDOW_RESIZE))
    {
        XSizeHints* sh = XAllocSizeHints();
        sh->flags = PMinSize | PMaxSize;
        sh->min_width = sh->max_width = width;
        sh->min_height = sh->max_height = height;
        XSetWMNormalHints(window.display, window.window, sh);
        XFree(sh);
    }
    XSetStandardProperties(window.display, window.window, title, NULL, 0, NULL, 0, NULL);
    XSelectInput(window.display, window.window, ExposureMask | ResizeRedirectMask | ButtonPressMask | KeyPressMask);
    Atom wmDeleteMessage = XInternAtom(window.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window.display, window.window, &wmDeleteMessage, 1);
    window.gc = XCreateGC(window.display, window.window, 0, NULL);
    /*XSetBackground(window.display, window.gc, 0x);
    XSetForeground(window.display, window.gc, black);*/
    XClearWindow(window.display, window.window);
    XMapRaised(window.display, window.window);

    window.renderer = XCreatePixmap(window.display, window.window, window.width, window.height, 24);
    window.vram = XCreatePixmap(window.display, window.window, window.width, window.height, 24);

    XSetForeground(window.display, window.gc, 0);
    XFillRectangle(window.display, window.renderer, window.gc, 0, 0, window.width, window.height);
    XFillRectangle(window.display, window.vram, window.gc, 0, 0, window.width, window.height);

    pthread_create(&thread, NULL, callback_runner, &thread_data);

    while(!window.is_killed)
    {
        XEvent event;

        if(!XNextEventTimeout(window.display, &event, 100))
        {
            continue;
        }
    
        if (event.type==Expose && event.xexpose.count==0)
        {
            pthread_mutex_lock(&(window.mutex));
            XCopyArea(window.display, window.renderer, window.window, window.gc, 0, 0, window.width, window.height, 0, 0);
            pthread_mutex_unlock(&(window.mutex));
        }
        if(event.type == ResizeRequest)
        {
            change_bitmap_size(&window, event.xresizerequest.width, event.xresizerequest.height);
        }
        if ((Atom)(event.xclient.data.l[0]) == wmDeleteMessage)
        {
            window.is_killed = SA_TRUE;
        }
    }
    XCloseDisplay(window.display);
    
    pthread_join(thread, NULL);
}