#include <X11/Xlib.h>
#include "SA/graphics/graphics.h"
#include "SA/graphics/x11/internal.h"


void SA_graphics_vram_draw_pixel(SA_GraphicsWindow* window, int x, int y, uint32_t color)
{
    pthread_mutex_lock(&(window->mutex));
    XSetForeground(window->display, window->gc, color);
    XDrawPoint(window->display, window->vram, window->gc, x, y);
    pthread_mutex_unlock(&(window->mutex));
}


void SA_graphics_vram_flush(SA_GraphicsWindow* window)
{
    pthread_mutex_lock(&(window->mutex));
    XCopyArea(window->display, window->vram, window->renderer, window->gc, 0, 0, window->width, window->height, 0, 0);
    XCopyArea(window->display, window->vram, window->window, window->gc, 0, 0, window->width, window->height, 0, 0);
    pthread_mutex_unlock(&(window->mutex));
}