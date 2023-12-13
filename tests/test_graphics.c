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

    SA_sleep(30e6);
}


void* other_window(void*)
{
    SA_graphics_create_window("le carré bleu !", 300, 200, 200, 200, 0, callback);
}

void test_graphics(void)
{
    pthread_t thread;

    pthread_create(&thread, NULL, other_window, callback);

    SA_graphics_create_window("Hello world", 0, 0, 300, 300, 0, callback);

    pthread_join(thread, NULL);
}