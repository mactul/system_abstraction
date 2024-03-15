#include <stdio.h>
#include "test.h"

void test_network(void)
{
    SA_RequestsHandler* handler = NULL;
    char buffer[1024];
    ssize_t n;

    for(int i =0; i < 3; i++)
    {
        size_t total = 20;
        
        handler = SA_req_get(handler, "http://coindesdevs.fr/test.bidule/test.php", "");

        if(handler == NULL)
        {
            SA_print_last_error();
            continue;
        }

        SA_req_display_headers(handler);

        SA_StreamHandler* stream = SA_stream_from_requests(handler);

        printf("%d\n", SA_stream_seek(stream, total));

        while((n = SA_stream_read(stream, buffer, 10)) > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer);
            total += n + 20;
            SA_stream_seek(stream, total);
        }

        putchar('\n');

        SA_stream_free(&stream);
    }

    SA_req_close_connection(&handler);
}