#include <stdio.h>
#include "test.h"

void test_network(void)
{
    SA_RequestsHandler* handler = NULL;
    char buffer[1024];
    int n;

    for(int i =0; i < 3; i++)
    {
        handler = SA_req_get(handler, "http://coindesdevs.fr/test.bidule/test.php", "");

        if(handler == NULL)
        {
            SA_print_last_error();
            continue;
        }

        SA_req_display_headers(handler);

        while((n = SA_req_read_output_body(handler, buffer, 1023)))
        {
            buffer[n] = '\0';
            printf("%s", buffer);
        }

        putchar('\n');
    }

    SA_req_close_connection(&handler);
}