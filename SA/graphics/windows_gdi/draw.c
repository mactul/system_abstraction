#include "SA/graphics/windows_gdi/internal.h"
#include "SA/graphics/graphics.h"

static inline COLORREF SA_color_to_windows_color(uint32_t color)
{
    uint32_t red = (color & 0xFF0000) >> 16;
    uint32_t green = (color & 0x00FF00) >> 8;
    uint32_t blue = (uint8_t)color;
    return RGB(red, green, blue);
}

void SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, int x, int y, uint32_t color)
{
    pthread_mutex_lock(&(window->mutex));
    SetPixel(window->vram, x, y, SA_color_to_windows_color(color));
    pthread_mutex_unlock(&(window->mutex));
}


void SA_graphics_vram_flush(SA_GraphicsWindow* window)
{
    PAINTSTRUCT ps;

    pthread_mutex_lock(&(window->mutex));
    BeginPaint(window->hwnd, &ps);
    BitBlt(window->hDC, 0, 0, window->width, window->height, window->vram, 0, 0, SRCCOPY);
    EndPaint(window->hwnd, &ps);
    pthread_mutex_unlock(&(window->mutex));
}