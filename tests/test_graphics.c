#include "test.h"
#include <stdio.h>
#include <pthread.h>

void callback(SA_GraphicsWindow* window)
{
    SA_graphics_vram_draw_line(window, 5, 5, 100, 100, 0xff7000, 3);
    SA_graphics_vram_flush(window);

    for(int i = 0; i < 10; i++)
    {
        SA_GraphicsEvent event;
        if(SA_graphics_wait_next_event(&event))
        {
            printf("%d %d\n", event.events.click.x, event.events.click.y);
        }
    }
}


void* other_window(void*)
{
    SA_sleep(1e6);
    SA_graphics_create_window("Hello world - threaded", 300, 200, 400, 400, 0, callback);

    return NULL;
}

void test_graphics(void)
{
    pthread_t thread;

    pthread_create(&thread, NULL, other_window, callback);

    SA_graphics_create_window("Hello world", 0, 0, 300, 300, 0, callback);


    pthread_join(thread, NULL);

    SA_graphics_create_window("machin", 0, 0, 300, 300, 0, callback);
}