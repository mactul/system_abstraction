#include <SDL2/SDL.h>
#include <SA/SA.h>
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/SDL2/sdl2_server.h"
#include "SA/graphics/SDL2/internal.h"


void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window), uint32_t events_to_handle)
{
    SA_GraphicsWindow window = {.is_killed = SA_FALSE, .height = height, .width = width, .mutex = PTHREAD_MUTEX_INITIALIZER, .events_to_handle = events_to_handle};
    SA_SDL_MsgCreateWindow msg = {.title = title, .pos_x = pos_x, .pos_y = pos_y, .flags = flags};
    SA_SDL_Message message = {.message_type = SA_SDL_CREATE_WINDOW, .window = &window, .msgs.create_window = msg};

    window.event_queue = SA_queue_create(sizeof(SA_GraphicsEvent), 1024);
    SA_sdl_create_window(&message);

    draw_callback(&window);

    SA_sdl_destroy_window(&window);
    pthread_mutex_destroy(&(window.mutex));
    SA_queue_free(&(window.event_queue));

    return;
}
