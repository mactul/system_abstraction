#include <math.h>
#include "SA/strings/strings.h"
#include "SA/graphics/graphics.h"
#include "SA/graphics/SDL2/sdl2_server.h"
#include "SA/graphics/SDL2/internal.h"

#define SIGN(x) ((x) < 0 ? -1: 1)

SA_bool SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t color)
{
    SA_bool ret = SA_TRUE;
    uint32_t* pixels;
    pthread_mutex_lock(&(window->mutex));
    if(window->is_killed)
    {
        ret = SA_FALSE;
        goto UNLOCK;
    }
    if(x >= window->width || y >= window->height)
    {
        ret = SA_FALSE;
        goto UNLOCK;
    }
    SDL_LockSurface(window->vram);
    pixels = window->vram->pixels;
    pixels[y * window->width + x] = color;
    SDL_UnlockSurface(window->vram);

UNLOCK:
    pthread_mutex_unlock(&(window->mutex));

    return ret;
}

SA_bool SA_graphics_vram_draw_horizontal_line(SA_GraphicsWindow* window, uint32_t x1, uint32_t x2, uint32_t y, uint32_t color, uint16_t thickness)
{
    SA_bool ret = SA_TRUE;
    uint32_t* pixels;

    pthread_mutex_lock(&(window->mutex));

    pixels = window->vram->pixels;

    if(x1 >= window->width || x2 >= window->width || y >= window->height)
    {
        ret = SA_FALSE;
        goto UNLOCK;
    }
    SDL_LockSurface(window->vram);
    if(x2 < x1)
    {
        uint32_t temp = x1;
        x1 = x2;
        x2 = temp;
    }
    for(uint16_t i = 0; i < thickness; i++)
    {
        SA_memset32(pixels + (y+i) * window->width + x1, color, x2 - x1);
    }

    SDL_UnlockSurface(window->vram);
UNLOCK:
    pthread_mutex_unlock(&(window->mutex));

    return ret;
}

SA_bool SA_graphics_vram_draw_vertical_line(SA_GraphicsWindow* window, uint32_t x, uint32_t y1, uint32_t y2, uint32_t color, uint16_t thickness)
{
    uint32_t index;
    SA_bool ret = SA_TRUE;
    uint32_t* pixels;

    pthread_mutex_lock(&(window->mutex));

    pixels = window->vram->pixels;

    if(x >= window->width || y1 >= window->height|| y2 >= window->height)
    {
        ret = SA_FALSE;
        goto UNLOCK;
    }
    SDL_LockSurface(window->vram);
    if(y2 < y1)
    {
        uint32_t temp = y1;
        y1 = y2;
        y2 = temp;
    }

    index = y1 * window->width;
    for(uint32_t i = 0; i < y2 -y1; i++)
    {
        SA_memset32(pixels + index + x, color, thickness);
        index += window->width;
    }

    SDL_UnlockSurface(window->vram);

UNLOCK:
    return ret;
}

static void graphics_bresenham(SA_GraphicsWindow* window, int x1, int y1, int x2, int y2, uint32_t color)
{
    int x = x1;
    int y = y1 * window->width;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int sx = SIGN(dx);
    int sy = SIGN(dy) * window->width;
    uint32_t* pixels = window->vram->pixels;
    
    dx = abs(dx);
    dy = abs(dy);

    pixels[y + x] = color;  // Without that, we can't be sure this particular pixel is drawn
    if(dx > dy)
    {
        int cumul = dx / 2;
        for(int i=1 ; i<dx ; i++)
        {
            x += sx;
            cumul += dy;
            if(cumul > dx)
            {
                cumul -= dx;
                y += sy;
            }
            pixels[y + x] = color;
        }
    }
    else
    {
        int cumul = dy / 2;
        for(int i=1 ; i<dy ; i++)
        {
            y += sy;
            cumul += dx;
            if(cumul > dy)
            {
                cumul -= dy;
                x += sx;
            }
            pixels[y + x] = color;
        }
    }
}

SA_bool SA_graphics_vram_draw_line(SA_GraphicsWindow* window, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint16_t thickness)
{
    if(x1 == x2)
    {
        return SA_graphics_vram_draw_vertical_line(window, x1, y1, y2, color, thickness);
    }
    if(y1 == y2)
    {
        return SA_graphics_vram_draw_horizontal_line(window, x1, x2, y1, color, thickness);
    }
    if((uint32_t)x1 >= window->width || (uint32_t)x2 >= window->width || (uint32_t)y1 >= window->width || (uint32_t)y2 >= window->width)
    {
        return SA_FALSE;
    }

    pthread_mutex_lock(&(window->mutex));
    SDL_LockSurface(window->vram);

    if(abs(x2-x1) > abs(y2 - y1))
    {
        int wy = (thickness-1) * sqrt((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1)) / fabs((double)(x2-x1));
        for(int i = 0; i <= wy; i++)
        {
            graphics_bresenham(window, x1, y1+i, x2, y2+i, color % ((uint32_t)-1));
        }
    }
    else
    {
        int wx = (thickness-1) * sqrt((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1)) / fabs((double)(y2-y1));
        for(int i = 0; i <= wx; i++)
        {
            graphics_bresenham(window, x1+i, y1, x2+i, y2, color % ((uint32_t)-1));
        }
    }

    SDL_UnlockSurface(window->vram);
    pthread_mutex_unlock(&(window->mutex));

    return SA_TRUE;
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