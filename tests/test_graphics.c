#include "test.h"
#include <pthread.h>

void callback(SA_GraphicsWindow* window)
{
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            SA_graphics_vram_draw_pixel(window, 100+i, 100+j, 0x00FFFF);
        }
    }
    SA_graphics_vram_flush(window);

    SA_sleep(5e6);
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