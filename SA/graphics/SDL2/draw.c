#include "SA/graphics/graphics.h"
#include "SA/graphics/SDL2/sdl2_server.h"
#include "SA/graphics/SDL2/internal.h"


void SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, int x, int y, uint32_t color)
{
    pthread_mutex_lock(&(window->mutex));
    if(window->is_killed)
    {
        return;
    }
    SDL_LockSurface(window->vram);
    uint32_t* pixels = window->vram->pixels;
    pixels[y * window->width + x] = color;
    SDL_UnlockSurface(window->vram);
    pthread_mutex_unlock(&(window->mutex));
}


void SA_graphics_vram_flush(SA_GraphicsWindow* window)
{
    if(window->is_killed)
    {
        return;
    }
    SDL_BlitSurface(window->vram, NULL, window->renderer, NULL);
    SA_sdl_redraw_window(window);
}