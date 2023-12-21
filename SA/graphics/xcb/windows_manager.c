#include <sys/select.h>
#include <pthread.h>
#include "SA/strings/strings.h"
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/graphics.h"
#include "SA/graphics/xcb/internal.h"

#define XCB_EVENT_MASK (~0x80)

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



void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window))
{
    SA_GraphicsWindow window = {.is_killed = SA_FALSE, .width = width, .height = height, .mutex = PTHREAD_MUTEX_INITIALIZER};

    pthread_t thread;
    ThreadData thread_data = {.window = &window, .draw_callback = draw_callback};

    xcb_point_t          points[] = {
            {10, 10},
            {10, 20},
            {20, 10},
            {20, 20}};

    window.connection = xcb_connect(NULL, NULL);

    xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(window.connection)).data;

    xcb_gcontext_t  foreground = xcb_generate_id (window.connection);
    uint32_t        mask       = XCB_GC_FOREGROUND | (XCB_GC_GRAPHICS_EXPOSURES & 0);
    uint32_t        values[2]  = {screen->black_pixel, 0};

    xcb_create_gc (window.connection, foreground, screen->root, mask, values);


    /* Create a window */
    window.window = xcb_generate_id (window.connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window (
        window.connection,             /* connection          */
        XCB_COPY_FROM_PARENT,          /* depth               */
        window.window,                 /* window Id           */
        screen->root,                  /* parent window       */
        pos_x, pos_y,                  /* x, y                */
        width, height,                 /* width, height       */
        10,                            /* border_width        */
        XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
        screen->root_visual,           /* visual              */
        mask, values                   /* masks               */
    );

    xcb_change_property(
        window.connection,
        XCB_PROP_MODE_REPLACE,
        window.window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        SA_strlen (title),
        title
    );

    
    xcb_map_window (window.connection, window.window);


    xcb_flush (window.connection);

    


    
    pthread_create(&thread, NULL, callback_runner, &thread_data);

    xcb_generic_event_t *event;
    while(!window.is_killed && (event = xcb_wait_for_event(window.connection)))
    {
        switch (event->response_type & XCB_EVENT_MASK)
        {
            case XCB_EXPOSE:
                xcb_poly_point (window.connection, XCB_COORD_MODE_ORIGIN, window.window, foreground, 4, points);
                xcb_flush (window.connection);
                printf("gros con !\n");
                break;
        }
        /* Make sure commands are sent before we pause so that the window gets shown */
        //xcb_flush (window.connection);

    }
    xcb_disconnect (window.connection);
    
    pthread_join(thread, NULL);
}