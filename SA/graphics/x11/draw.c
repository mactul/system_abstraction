#include <X11/Xlib.h>
#include "SA/graphics/graphics.h"
#include "SA/graphics/x11/internal.h"


SA_bool SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t color)
{
    pthread_mutex_lock(&(window->mutex));
    XSetForeground(window->display, window->gc, color);
    XDrawPoint(window->display, window->vram, window->gc, x, y);
    pthread_mutex_unlock(&(window->mutex));

    return SA_TRUE;
}

SA_bool SA_graphics_vram_draw_line(SA_GraphicsWindow* window, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, uint16_t thickness)
{
    pthread_mutex_lock(&(window->mutex));
    XSetForeground(window->display, window->gc, color);
    XSetLineAttributes(window->display, window->gc, thickness, LineSolid, CapNotLast, JoinMiter);
    XDrawLine(window->display, window->vram, window->gc, x1, y1, x2, y2);
    pthread_mutex_unlock(&(window->mutex));

    return SA_TRUE;
}

void SA_graphics_vram_flush(SA_GraphicsWindow* window)
{
    pthread_mutex_lock(&(window->mutex));
    XCopyArea(window->display, window->vram, window->renderer, window->gc, 0, 0, window->width, window->height, 0, 0);
    XCopyArea(window->display, window->vram, window->window, window->gc, 0, 0, window->width, window->height, 0, 0);
    pthread_mutex_unlock(&(window->mutex));
}