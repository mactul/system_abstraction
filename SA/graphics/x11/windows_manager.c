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

static void destroy_window(SA_GraphicsWindow* window)
{
    pthread_mutex_lock(&(window->mutex));
    if(window->display == 0)
    {
        goto UNLOCK;
    }
    XFreePixmap(window->display, window->vram);
    XFreePixmap(window->display, window->renderer);
    XFreeGC(window->display, window->gc);
    XCloseDisplay(window->display);
    window->window = 0;
    window->display = 0;
    window->gc = 0;
    window->renderer = 0;
    window->vram = 0;
    window->height = 0;
    window->width = 0;
    window->is_killed = SA_TRUE;

UNLOCK:
    pthread_mutex_unlock(&(window->mutex));
}

static inline void SA_graphics_post_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event)
{
    if(window != NULL && window->event_queue != NULL && !window->is_killed && event->event_type != SA_GRAPHICS_EVENT_NOTHING)
    {
        SA_queue_push(window->event_queue, event);
    }
}

SA_bool SA_graphics_poll_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event)
{
    return SA_queue_pull(window->event_queue, event, SA_FALSE);
}

SA_bool SA_graphics_wait_next_event(SA_GraphicsWindow* window, SA_GraphicsEvent* event)
{
    return SA_queue_pull(window->event_queue, event, SA_TRUE);
}


static void* callback_runner(void* data)
{
    ThreadData* thread_data = (ThreadData*)data;

    thread_data->draw_callback(thread_data->window);

    destroy_window(thread_data->window);

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
    SA_GraphicsWindow window = {.is_killed = SA_FALSE, .width = width, .height = height, .mutex = PTHREAD_MUTEX_INITIALIZER, .events_to_queue = events_to_queue, .event_callback = event_callback};
    pthread_t thread;
    ThreadData thread_data = {.window = &window, .draw_callback = draw_callback};

    window.display = XOpenDisplay(NULL);

    window.event_queue = SA_queue_create(sizeof(SA_GraphicsEvent), SA_GRAPHICS_EVENT_QUEUE_LENGTH);

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
    XSelectInput(window.display, window.window, ExposureMask | ResizeRedirectMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | PointerMotionMask);
    Atom wmDeleteMessage = XInternAtom(window.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window.display, window.window, &wmDeleteMessage, 1);
    window.gc = XCreateGC(window.display, window.window, 0, NULL);
    /*XSetBackground(window.display, window.gc, 0x);
    XSetForeground(window.display, window.gc, black);*/
    XClearWindow(window.display, window.window);
    XMapRaised(window.display, window.window);
    XMoveWindow(window.display, window.window, pos_x, pos_y);

    window.renderer = XCreatePixmap(window.display, window.window, window.width, window.height, 24);
    window.vram = XCreatePixmap(window.display, window.window, window.width, window.height, 24);

    XSetForeground(window.display, window.gc, 0);
    XFillRectangle(window.display, window.renderer, window.gc, 0, 0, window.width, window.height);
    XFillRectangle(window.display, window.vram, window.gc, 0, 0, window.width, window.height);

    pthread_create(&thread, NULL, callback_runner, &thread_data);

    while(!window.is_killed)
    {
        SA_GraphicsEvent graphics_event = {.event_type = SA_GRAPHICS_EVENT_NOTHING};
        XEvent event;

        if(!XNextEventTimeout(window.display, &event, 100))
        {
            continue;
        }

        switch(event.type)
        {
            case ClientMessage:
                if ((Atom)(event.xclient.data.l[0]) == wmDeleteMessage)
                {
                    graphics_event.event_type = SA_GRAPHICS_EVENT_CLOSE_WINDOW;
                    SA_graphics_post_event(&window, &graphics_event);
                    destroy_window(&window);
                }
                break;

            case Expose:
                if (event.xexpose.count == 0)
                {
                    pthread_mutex_lock(&(window.mutex));
                    XCopyArea(window.display, window.renderer, window.window, window.gc, 0, 0, window.width, window.height, 0, 0);
                    pthread_mutex_unlock(&(window.mutex));
                }
                break;

            case ResizeRequest:
                change_bitmap_size(&window, event.xresizerequest.width, event.xresizerequest.height);
                break;

            case ButtonPress:
                switch(event.xbutton.button)
                {
                    case Button1:
                        graphics_event.event_type = SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN;
                        graphics_event.events.mouse.x = event.xbutton.x;
                        graphics_event.events.mouse.y = event.xbutton.y;
                        if((window.events_to_queue & SA_GRAPHICS_QUEUE_MOUSE_CLICK) == SA_GRAPHICS_QUEUE_MOUSE_CLICK)
                        {
                            SA_graphics_post_event(&window, &graphics_event);
                        }
                        break;
                    
                    case Button4:
                        graphics_event.event_type = SA_GRAPHICS_EVENT_SCROLL_UP;
                        if((window.events_to_queue & SA_GRAPHICS_QUEUE_SCROLL) == SA_GRAPHICS_QUEUE_SCROLL)
                        {
                            SA_graphics_post_event(&window, &graphics_event);
                        }
                        break;
                    case Button5:
                        graphics_event.event_type = SA_GRAPHICS_EVENT_SCROLL_DOWN;
                        if((window.events_to_queue & SA_GRAPHICS_QUEUE_SCROLL) == SA_GRAPHICS_QUEUE_SCROLL)
                        {
                            SA_graphics_post_event(&window, &graphics_event);
                        }
                        break;

                }
                break;
            case ButtonRelease:
                switch(event.xbutton.button)
                {
                    case Button1:
                        graphics_event.event_type = SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP;
                        graphics_event.events.mouse.x = event.xbutton.x;
                        graphics_event.events.mouse.y = event.xbutton.y;
                        if((window.events_to_queue & SA_GRAPHICS_QUEUE_MOUSE_CLICK) == SA_GRAPHICS_QUEUE_MOUSE_CLICK)
                        {
                            SA_graphics_post_event(&window, &graphics_event);
                        }
                        break;
                }
                break;
            case MotionNotify:
                graphics_event.event_type = SA_GRAPHICS_EVENT_MOUSE_MOVE;
                graphics_event.events.mouse.x = event.xbutton.x;
                graphics_event.events.mouse.y = event.xbutton.y;
                if((window.events_to_queue & SA_GRAPHICS_QUEUE_MOUSE_MOVE) == SA_GRAPHICS_QUEUE_MOUSE_MOVE)
                {
                    SA_graphics_post_event(&window, &graphics_event);
                }
                break;
        }

        if(graphics_event.event_type == SA_GRAPHICS_EVENT_NOTHING)
            continue;
        
        
        if(window.event_callback != NULL)
            window.event_callback(&window, &graphics_event);
    }
    
    pthread_join(thread, NULL);

    SA_queue_free(&(window.event_queue));
}