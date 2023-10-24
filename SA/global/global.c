#include "SA/global/global.h"
#include "SA/memory/mem_alloc.h"
#include "SA/network/easy_tcp_tls.h"
#include "SA/random/random.h"

/*
This function MUST BE put at the top of the main function before doing anything else with the library.
*/
void SA_init(void)
{
    #ifndef SA_NETWORK_DISABLED
        _SA_socket_start();
    #endif
    SA_set_unsecure_seed(SA_random_standard_seed());
}

/*
This function must be put at the end of the program, after having closed all the threads.
In debug mode, it can display warning messages on stderr.
*/
void SA_destroy(void)
{
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