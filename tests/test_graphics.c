#include "test.h"
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "SA/graphics/universal_font/universal_font.h"



void callback(SA_GraphicsWindow* window)
{
    SA_bool event_readed;
    SA_GraphicsEvent event;
    SA_GraphicsRectangle draw_area = {.top_left_corner_x = 50, .top_left_corner_y = 100, .height = 300, .width = 300};
    double x_array[200];
    double y_array[200];

    for(unsigned int i = 0; i < sizeof(x_array)/sizeof(double); i++)
    {
        x_array[i] = 0.05*i;
        y_array[i] = sin(0.05*i);
    }
    //SA_graphics_vram_draw_pixel(window, 5, 5, 0xff7000);
    SA_graphics_vram_draw_line(window, 5, 5, 100, 100, 0xff7000, 3);
    SA_graphics_vram_draw_vertical_line(window, 10, 10, 20, 255, 2);
    SA_graphics_vram_draw_text(window, 20, 20, "Bonjour, j'aime le beurre", 0x0000ff);

    SA_graphics_plot_continuous_graph(window, x_array, y_array, sizeof(x_array)/sizeof(double), &draw_area, 0x000000, 0xff0000, 0xffffff);
    

    SA_graphics_vram_flush(window);
    do
    {
        if((event_readed = SA_graphics_wait_next_event(window, &event)))
        {
            switch(event.event_type)
            {
                case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN:
                    printf("click down: %d %d\n", event.events.mouse.x, event.events.mouse.y);
                    break;
                case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP:
                    printf("click up: %d %d\n", event.events.mouse.x, event.events.mouse.y);
                    break;
                case SA_GRAPHICS_EVENT_SCROLL_UP:
                    printf("scroll up\n");
                    break;
                case SA_GRAPHICS_EVENT_SCROLL_DOWN:
                    printf("scroll down\n");
                    break;
                case SA_GRAPHICS_EVENT_CLOSE_WINDOW:
                    printf("close window\n");
                    break;
                case SA_GRAPHICS_EVENT_KEY_DOWN:
                    printf("key down: %s\n", event.events.key.str);
                    break;
                default:
                    printf("unknown event\n");
                    break;
            }
        }
    } while (!event_readed || event.event_type != SA_GRAPHICS_EVENT_CLOSE_WINDOW);
}


void* other_window(void*)
{
    SA_graphics_create_window("Hello world - Threaded", 200, 200, 500, 500, 0, callback, SA_GRAPHICS_QUEUE_MOUSE_CLICK, NULL);

    return NULL;
}

void event_callback(SA_GraphicsWindow*, SA_GraphicsEvent* event)
{
    switch(event->event_type)
    {
        case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN:
            printf("event_callback: click down: %d %d\n", event->events.mouse.x, event->events.mouse.y);
            break;
        case SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_UP:
            printf("event_callback: click up: %d %d\n", event->events.mouse.x, event->events.mouse.y);
            break;
        case SA_GRAPHICS_EVENT_SCROLL_UP:
            printf("event_callback: scroll up\n");
            break;
        case SA_GRAPHICS_EVENT_SCROLL_DOWN:
            printf("event_callback: scroll down\n");
            break;
        case SA_GRAPHICS_EVENT_CLOSE_WINDOW:
            printf("event_callback: close window\n");
            break;
        case SA_GRAPHICS_EVENT_MOUSE_MOVE:
            printf("event_callback: move: %d %d\n", event->events.mouse.x, event->events.mouse.y);
            break;
        case SA_GRAPHICS_EVENT_KEY_DOWN:
            printf("event_callback: key down: %s\n", event->events.key.str);
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

    SA_graphics_create_window("Hello world", 100, 100, 600, 600, 0, callback, SA_GRAPHICS_QUEUE_MOUSE_CLICK | SA_GRAPHICS_QUEUE_SCROLL | SA_GRAPHICS_QUEUE_KEYBOARD, NULL);


    //pthread_join(thread, NULL);

    SA_graphics_create_window("machin", 0, 0, 600, 600, 0, callback, 0, event_callback);
}