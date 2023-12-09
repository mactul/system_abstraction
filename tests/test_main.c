#include <stdio.h>
#include "test.h"

int main()
{
    SA_init();

    #ifndef DEBUG
        fprintf(stderr, "/!\\ WARNING /!\\ : This program is useless in release mode, compile it in debug mode\n");
    #endif

    SA_graphics_create_window("Hello world", 0, 0, 500, 300, 0);

    SA_recursive_mkdir("bidule/machin/chose");

    test_strings();
    test_path();
    test_parser();
    test_math();
    test_memory();
    test_network();
    
    SA_destroy();
}
