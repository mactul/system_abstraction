#include <SDL2/SDL.h>
#include <SA/SA.h>
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/SDL2/sdl2_server.h"
#include "SA/graphics/SDL2/internal.h"


void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags, void (*draw_callback)(SA_GraphicsWindow* window))
{
    SA_GraphicsWindow window = {.is_killed = SA_FALSE, .height = height, .width = width, .mutex = PTHREAD_MUTEX_INITIALIZER};
    SA_SDL_MsgCreateWindow msg = {.title = title, .pos_x = pos_x, .pos_y = pos_y, .width = width, .height = height, .flags = flags, .draw_callback = draw_callback};
    SA_SDL_Message message = {.message_type = SA_SDL_CREATE_WINDOW, .window = &window, .msgs.create_window = msg};

    pthread_mutex_init(&(window.mutex), NULL);
    SA_sdl_post_event(&message);

    draw_callback(&window);

    SA_sdl_destroy_window(&window);

    return;
}
