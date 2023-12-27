#ifndef SA_GRAPHICS_SDL2_INTERNAL
    #define SA_GRAPHICS_SDL2_INTERNAL

    #include "SA/global/global.h"

    #include <SDL2/SDL.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include "SA/memory/dynamic_array.h"
    #include "SA/graphics/graphics.h"


    enum SA_SDL_MESSAGES_TYPES {
        SA_SDL_STOP_THREAD,
        SA_SDL_CREATE_WINDOW,
        SA_SDL_DESTROY_WINDOW,
        SA_SDL_REDRAW_WINDOW
    };

    struct _SA_graphics_window {
        SDL_Window* window;
        SDL_Surface* renderer;  // This backbuffer is required to handle the "expose" event.
        SDL_Surface* vram;
        SA_DynamicArray* event_queue;
        pthread_mutex_t mutex;
        uint32_t width;
        uint32_t height;
        SA_bool is_killed;
    };

    typedef struct _sa_sdl_msg_create_window {
        sem_t* window_is_created;
        const char* title;
        int pos_x;
        int pos_y;
        uint32_t flags;
    } SA_SDL_MsgCreateWindow;

    typedef struct _sa_sdl_msg_destroy_window {
        sem_t* window_is_destroyed;
    } SA_SDL_MsgDestroyWindow;

    typedef struct _sa_sdl_msg_redraw_window {
        sem_t* window_is_redrawn;
    } SA_SDL_MsgRedrawWindow;

    typedef struct _sa_sdl_message {
        enum SA_SDL_MESSAGES_TYPES message_type;
        SA_GraphicsWindow* window;
        union msgs {
            SA_SDL_MsgCreateWindow create_window;
            SA_SDL_MsgDestroyWindow destroy_window;
            SA_SDL_MsgRedrawWindow redraw_window;
        } msgs;
    } SA_SDL_Message;

#endif