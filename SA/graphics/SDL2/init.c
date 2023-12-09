#include <SDL2/SDL.h>
#include "SA/graphics/internal_init.h"

void _SA_graphics_init(void)
{
    (void)SDL_Init(SDL_INIT_VIDEO);
}

void _SA_graphics_destroy(void)
{
    SDL_Quit();
}