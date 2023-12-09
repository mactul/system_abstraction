#include <SDL2/SDL.h>
#include <pthread.h>
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/graphics.h"

struct _SA_graphics_window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    pthread_mutex_t mutex;
};


void* eventHandler(void* data)
{
    SA_GraphicsWindow* windows = (SA_GraphicsWindow*)data;
    
    pthread_mutex_lock(&(windows->mutex));
        SDL_SetRenderDrawColor(windows->renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(windows->renderer, 10, 10, 100, 100);
        SDL_SetRenderDrawColor(windows->renderer, 255, 255, 255, 255);
    pthread_mutex_unlock(&(windows->mutex));

    return 0;
}

void SA_graphics_create_window(const char* title, int pos_x, int pos_y, int width, int height, uint32_t flags)
{
    SA_GraphicsWindow windows;
    
    windows.window = SDL_CreateWindow(title, pos_x, pos_y, width, height, flags | SDL_WINDOW_SHOWN);
    if(windows.window == NULL)
    {
        return;
    }
    windows.renderer = SDL_CreateRenderer(windows.window, -1, SDL_RENDERER_SOFTWARE);
    if(windows.renderer == NULL)
    {
        SDL_DestroyWindow(windows.window);
        return;
    }

    pthread_mutex_init(&(windows.mutex), NULL);

    SDL_Event events;
    SA_bool isOpen = SA_TRUE;

    SDL_SetRenderDrawColor(windows.renderer, 255, 255, 255, 255);
    SDL_RenderClear(windows.renderer);

    pthread_t thread;

    pthread_create(&thread, NULL, eventHandler, &windows);

    while (isOpen)
    {
        pthread_mutex_lock(&(windows.mutex));
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = SA_FALSE;
                break;
            }
        }

        SDL_RenderPresent(windows.renderer);

        pthread_mutex_unlock(&(windows.mutex));
    }

    pthread_mutex_destroy(&(windows.mutex));
    SDL_DestroyRenderer(windows.renderer);
    SDL_DestroyWindow(windows.window);

    return;
}
