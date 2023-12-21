/***********************************************************************************/ 
/* SDL2 is a BIG PIECE OF SHIT, so this file is required to synchronise everything */
/***********************************************************************************/

#include "SA/memory/dynamic_array.h"
#include "SA/graphics/SDL2/internal.h"
#include "SA/graphics/SDL2/sdl2_server.h"
#include <SA/SA.h>


static SA_DynamicArray* _SA_SDL_messages = NULL;
static pthread_mutex_t _SA_SDL_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint32_t SDL_CUSTOM_EVENT;

static SA_DynamicArray* _SA_windows = NULL;  // SA_GraphicsWindow* indexed by windowID


void SA_sdl_post_event(SA_SDL_Message* message)
{
    pthread_mutex_lock(&_SA_SDL_queue_mutex);
    if(_SA_SDL_messages != NULL)
    {
        SDL_Event event = {.type = SDL_CUSTOM_EVENT};
        SA_dynarray_append(SA_SDL_Message, _SA_SDL_messages, *message);

        SDL_PushEvent(&event);
    }
    pthread_mutex_unlock(&_SA_SDL_queue_mutex);
}

SA_bool SA_sdl_get_next_event(SA_SDL_Message* message)
{
    if(SA_dynarray_size(_SA_SDL_messages) <= 0)
    {
        return SA_FALSE;
    }
    *message = SA_dynarray_get(SA_SDL_Message, _SA_SDL_messages, 0);
    SA_dynarray_remove_block(_SA_SDL_messages, 0, 1);
    
    return SA_TRUE;
}

void SA_sdl_stop_thread(void)
{
    SA_SDL_Message message = {.message_type = SA_SDL_STOP_THREAD};
    SA_sdl_post_event(&message);
}

void SA_sdl_destroy_window(SA_GraphicsWindow* window)
{
    sem_t window_is_destroyed;
    SA_SDL_Message message = {.message_type = SA_SDL_DESTROY_WINDOW, .window = window, .msgs.destroy_window = {.window_is_destroyed = &window_is_destroyed}};
    
    sem_init(&window_is_destroyed, 0, 0);
    SA_sdl_post_event(&message);

    sem_wait(&window_is_destroyed);

    sem_destroy(&window_is_destroyed);
}

static void create_window(SA_GraphicsWindow* window, SA_SDL_MsgCreateWindow* msg)
{
    window->window = SDL_CreateWindow(msg->title, msg->pos_x, msg->pos_y, msg->width, msg->height, msg->flags | SDL_WINDOW_SHOWN);
    if(window->window == NULL)
    {
        window->is_killed = SA_TRUE;
        return;
    }
    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_SOFTWARE);
    if(window->renderer == NULL)
    {
        SDL_DestroyWindow(window->window);
        window->window = NULL;
        window->is_killed = SA_TRUE;
    }
    SA_dynarray_set(SA_GraphicsWindow*, _SA_windows, SDL_GetWindowID(window->window), window);
    SDL_RenderClear(window->renderer);
}

static void destroy_window(SA_GraphicsWindow* window)
{
    pthread_mutex_lock(&(window->mutex));
    SDL_DestroyRenderer(window->renderer);
    window->renderer = NULL;
    SDL_DestroyWindow(window->window);
    window->window = NULL;
    window->height = 0;
    window->width = 0;
    window->is_killed = SA_TRUE;
    pthread_mutex_unlock(&(window->mutex));
}

static SA_bool custom_event_switch(void)
{
    SA_SDL_Message message;
    SA_bool ret = SA_TRUE;

    pthread_mutex_lock(&_SA_SDL_queue_mutex);
    SA_sdl_get_next_event(&message);

    switch (message.message_type)
    {
        case SA_SDL_CREATE_WINDOW:
            pthread_mutex_lock(&(message.window->mutex));
            create_window(message.window, &(message.msgs.create_window));
            pthread_mutex_unlock(&(message.window->mutex));
            break;
        case SA_SDL_DESTROY_WINDOW:
            destroy_window(message.window);
            sem_post(message.msgs.destroy_window.window_is_destroyed);
            break;
        case SA_SDL_STOP_THREAD:
            ret = SA_FALSE;
            goto UNLOCK;
        default:
            break;
    }

UNLOCK:
    pthread_mutex_unlock(&_SA_SDL_queue_mutex);
    return ret;
}

static SA_bool sdl_event_switch(SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_WINDOWEVENT:
            SA_GraphicsWindow* window = SA_dynarray_get(SA_GraphicsWindow*, _SA_windows, event->window.windowID);
            switch(event->window.event)
            {
                case SDL_WINDOWEVENT_EXPOSED:
                    pthread_mutex_lock(&(window->mutex));
                    SDL_RenderPresent(window->renderer);
                    pthread_mutex_unlock(&(window->mutex));
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    destroy_window(window);
                    break;
            }
            break;
        
        case SDL_QUIT:
            return SA_TRUE;
        default:
            break;
    }

    return SA_TRUE;
}

void* SA_sdl_server_thread(void* init_finished)
{
    SDL_Event sdl_event;
    SA_bool server_running = SA_TRUE;

    (void)SDL_Init(SDL_INIT_VIDEO);
    _SA_SDL_messages = SA_dynarray_create_size_hint(SA_SDL_Message, 10);
    _SA_windows = SA_dynarray_create(SA_GraphicsWindow*);
    SDL_CUSTOM_EVENT = SDL_RegisterEvents(1);

    sem_post((sem_t*)init_finished);

    while(server_running)
    {
        SDL_WaitEvent(&sdl_event);
        if(sdl_event.type == SDL_CUSTOM_EVENT)
        {
            server_running = custom_event_switch();
        }
        else
        {
            server_running = sdl_event_switch(&sdl_event);
        }
        
    }
    pthread_mutex_destroy(&_SA_SDL_queue_mutex);
    SA_dynarray_free(&_SA_SDL_messages);
    SA_dynarray_free(&_SA_windows);

    SDL_Quit();

    return NULL;
}