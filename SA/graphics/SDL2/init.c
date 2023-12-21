#include <SDL2/SDL.h>
#include "SA/graphics/internal_init.h"
#include "SA/graphics/SDL2/sdl2_server.h"

static pthread_t thread;

void _SA_graphics_init(void)
{
    sem_t init_finished;

    sem_init(&init_finished, 0, 0);

    pthread_create(&thread, NULL, SA_sdl_server_thread, &init_finished);

    sem_wait(&init_finished);

    sem_destroy(&init_finished);
}

void _SA_graphics_destroy(void)
{
    SA_sdl_stop_thread();

    pthread_join(thread, NULL);
}