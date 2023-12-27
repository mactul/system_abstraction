#include "test.h"
#include <stdio.h>
#include <pthread.h>

void callback(SA_GraphicsWindow* window)
{
    SA_bool event_readed;
    SA_GraphicsEvent event;
    SA_graphics_vram_draw_line(window, 5, 5, 100, 100, 0xff7000, 3);
    SA_graphics_vram_flush(window);

    do
    {
        if((event_readed = SA_graphics_wait_next_event(window, &event)))
        {
            printf("%d %d\n", event.events.click.x, event.events.click.y);
        }
    } while (!event_readed || event.event_type != SA_GRAPHICS_EVENT_CLOSE_WINDOW);
}


void* other_window(void*)
{
    SA_graphics_create_window("Hello world - threaded", 300, 200, 400, 400, 0, callback, SA_GRAPHICS_HANDLE_EVERYTHING);

    return NULL;
}

void test_graphics(void)
{
    pthread_t thread;

    pthread_create(&thread, NULL, other_window, NULL);

    SA_graphics_create_window("Hello world", 0, 0, 300, 300, 0, callback, SA_GRAPHICS_HANDLE_MOUSE);


    pthread_join(thread, NULL);

    SA_graphics_create_window("machin", 0, 0, 300, 300, 0, callback, 0);
}