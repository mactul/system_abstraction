#ifndef SA_GRAPHICS_SDL2_SERVER
    #define SA_GRAPHICS_SDL2_SERVER

    #include "SA/global/global.h"

    #include "SA/graphics/SDL2/internal.h"


    void SA_sdl_post_event(SA_SDL_Message* message);
    SA_bool SA_sdl_get_next_event(SA_SDL_Message* message);
    void SA_sdl_stop_thread(void);
    void* SA_sdl_server_thread(void* data);
    void SA_sdl_destroy_window(SA_GraphicsWindow* window);

#endif