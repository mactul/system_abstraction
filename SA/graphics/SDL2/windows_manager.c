#include <SDL2/SDL.h>
#include <SA/SA.h>
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/SDL2/sdl2_server.h"
#include "SA/graphics/SDL2/internal.h"


void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window), uint32_t events_to_queue, void (*event_callback)(SA_GraphicsWindow* window, SA_GraphicsEvent* event))
{
    sem_t window_is_completly_destroyed;
    SA_GraphicsWindow window = {
        .is_killed = SA_FALSE,
        .height = height,
        .width = width,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .events_to_queue = events_to_queue,
        .event_callback = event_callback,
        .is_completly_destroyed = &window_is_completly_destroyed
    };
    SA_SDL_MsgCreateWindow msg = {.title = title, .pos_x = pos_x, .pos_y = pos_y, .flags = flags};
    SA_SDL_Message message = {.message_type = SA_SDL_CREATE_WINDOW, .window = &window, .msgs.create_window = msg};

    sem_init(&window_is_completly_destroyed, 0, 0);
    window.event_queue = SA_queue_create(sizeof(SA_GraphicsEvent), SA_GRAPHICS_EVENT_QUEUE_LENGTH);
    SA_sdl_create_window(&message);

    if(draw_callback != NULL)
        draw_callback(&window);
    else
        sem_wait(&window_is_completly_destroyed);

    sem_destroy(&window_is_completly_destroyed);
    SA_sdl_destroy_window(&window);
    pthread_mutex_destroy(&(window.mutex));
    SA_queue_free(&(window.event_queue));

    return;
}
