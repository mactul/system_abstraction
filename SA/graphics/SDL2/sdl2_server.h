#ifndef SA_GRAPHICS_SDL2_SERVER
    #define SA_GRAPHICS_SDL2_SERVER

    #include "SA/global/global.h"

    #include "SA/graphics/SDL2/internal.h"

    /**
     * @brief Send a custom event to the event queue
     * 
     * @param message Pointer to the SA_SDL_Message structure related to this message
     */
    void SA_sdl_post_event(SA_SDL_Message* message);

    /**
     * @brief Get the next event from the event queue
     * 
     * @param message Pointer to the SA_SDL_Message structure that will be filled with the event information
     * @return - SA_FALSE if there is no event to handle
     * @return - SA_TRUE if there is an event to handle
     */
    SA_bool SA_sdl_get_next_event(SA_SDL_Message* message);

    /**
     * @brief Send a message in the event queue to stop the SDL server
     */
    void SA_sdl_stop_thread(void);

    /**
     * @brief Start the SDL server
     * 
     * @param data Pointer to a sem_t object containing the state of initialization
     * @return NULL
     */
    void* SA_sdl_server_thread(void* data);

    /**
     * @brief Close a graphical window
     * 
     * @param window Pointer to the SA_GraphicsWindow structure related to this window
     */
    void SA_sdl_destroy_window(SA_GraphicsWindow* window);

    /**
     * @brief Redraw a graphical window
     * 
     * @param window Pointer to the SA_GraphicsWindow structure related to this window
     */
    void SA_sdl_redraw_window(SA_GraphicsWindow* window);

    /**
     * @brief Create a graphical window
     * 
     * @param message Pointer to the SA_SDL_Message structure related to this window
     */
    void SA_sdl_create_window(SA_SDL_Message* message);
    
#endif