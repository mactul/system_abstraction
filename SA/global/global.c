#include "SA/global/global.h"
#include "SA/memory/mem_alloc.h"
#include "SA/network/easy_tcp_tls.h"
#include "SA/random/random.h"
#include "SA/graphics/internal_init.h"

#ifdef DEBUG
#include <stdlib.h>

SA_bool _SA_is_init = SA_FALSE;

static void verify_SA_destroy(void)
{
    if(_SA_is_init)
    {
        SA_print_error("DebugWarning: SA_destroy was never called\n");
    }
}
#endif

/*
This function MUST BE put at the top of the main function before doing anything else with the library.
*/
void SA_init(void)
{
    #ifdef DEBUG
        _SA_is_init = SA_TRUE;
        atexit(verify_SA_destroy);
    #endif

    #ifndef SA_NETWORK_DISABLED
        _SA_socket_start();
    #endif

    #ifndef SA_GRAPHICS_DISABLED
        _SA_graphics_init();
    #endif
}

/*
This function must be put at the end of the program, after having closed all the threads.
In debug mode, it can display warning messages on stderr.
*/
void SA_destroy(void)
{
    #ifdef DEBUG
        _SA_is_init = SA_FALSE;
    #endif

    #ifndef SA_GRAPHICS_DISABLED
        #ifdef SA_GRAPHICS_SDL2
            _SA_graphics_destroy();
        #endif
    #endif

    #ifdef SA_MEMORY_DEBUG
    if(!_SA_is_everything_freed())
    {
        SA_print_error(" /!\\ WARNING /!\\ : Not all SA_malloc were freed\n");
    }
    #endif

    #ifndef SA_NETWORK_DISABLED
        _SA_socket_cleanup();
    #endif
}