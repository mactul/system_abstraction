#include "test.h"
#include <stdio.h>
//#include <pthread.h>
#include "SA/graphics/universal_font/universal_font.h"



void callback(SA_GraphicsWindow* window)
{
    SA_bool event_readed;
    SA_GraphicsEvent event;
    //SA_graphics_vram_draw_pixel(window, 5, 5, 0xff7000);
    SA_graphics_vram_draw_line(window, 5, 5, 100, 100, 0xff7000, 3);
    SA_graphics_vram_flush(window);

    /*do
    {
        if((event_readed = SA_graphics_wait_next_event(window, &event)))
        {
            switch(event.event_type)
            {
                case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN:
                    printf("click down: %d %d\n", event.events.click.x, event.events.click.y);
                    break;
                case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP:
                    printf("click up: %d %d\n", event.events.click.x, event.events.click.y);
                    break;
                case SA_GRAPHICS_EVENT_CLOSE_WINDOW:
                    printf("close window\n");
                    break;
                default:
                    printf("unknown event\n");
                    break;
            }
        }
    } while (!event_readed || event.event_type != SA_GRAPHICS_EVENT_CLOSE_WINDOW);*/

    SA_sleep(1e6);
}


/*void* other_window(void*)
{
    SA_graphics_create_window("Hello world - threaded", 300, 200, 400, 400, 0, callback, SA_GRAPHICS_HANDLE_EVERYTHING);

    return NULL;
}*/

void event_callback(SA_GraphicsWindow*, SA_GraphicsEvent* event)
{
    switch(event->event_type)
    {
        case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN:
            printf("event_callback: click down: %d %d\n", event->events.click.x, event->events.click.y);
            break;
        case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP:
            printf("event_callback: click up: %d %d\n", event->events.click.x, event->events.click.y);
            break;
        case SA_GRAPHICS_EVENT_CLOSE_WINDOW:
            printf("event_callback: close window\n");
            break;
        default:
            printf("event_callback: unknown event\n");
            break;
    }
    
    return;
}

void test_graphics(void)
{
    //pthread_t thread;

    //pthread_create(&thread, NULL, other_window, NULL);

    //SA_UniversalFont font;
    //SA_UF_load_psf_font_from_file(&font, "font_psf1.psf");

    SA_graphics_create_window("Hello world", 0, 0, 300, 300, 0, callback, SA_GRAPHICS_QUEUE_MOUSE, NULL);


    //pthread_join(thread, NULL);

    SA_graphics_create_window("machin", 0, 0, 300, 300, 0, callback, 0, event_callback);
}