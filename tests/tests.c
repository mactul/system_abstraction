#include <stdio.h>
#include <assert.h>
#include "SA/SA.h"

int main()
{
    char str1[] = "bonjour";
    char str2[256];
    int i;

    SA_init();

    assert(SA_CHAR_IS_LOWERCASE('c') == SA_TRUE);
    assert(SA_CHAR_IS_LOWERCASE('H') == SA_FALSE);
    assert(SA_CHAR_IS_LOWERCASE(';') == SA_FALSE);

    assert(SA_CHAR_IS_UPPERCASE('K') == SA_TRUE);
    assert(SA_CHAR_IS_UPPERCASE('i') == SA_FALSE);
    assert(SA_CHAR_IS_UPPERCASE(';') == SA_FALSE);

    SA_str_to_upper_inplace(str1);

    assert(SA_strcmp(str1, "bonjour") != 0);
    assert(SA_strcmp(str1, "BONJOUR") == 0);
    assert(SA_strcasecmp(str1, "bonjour") == 0);
    assert(SA_strcasecmp(str1, "bOnJOuR") == 0);

    assert(SA_strcmp("airplane", "zoro") < 0);
    assert(SA_strcmp("airplane", "ZoRo") > 0);

    assert(SA_strcasecmp("airplane", "zoro") < 0);
    assert(SA_strcasecmp("airplane", "ZoRo") < 0);

    assert(SA_strlen("bonjour") == 7);
    assert(SA_strlen("") == 0);

    assert(SA_toupper('a') == 'A');
    assert(SA_toupper('B') == 'B');

    assert(SA_tolower('a') == 'a');
    assert(SA_tolower('B') == 'b');

    SA_str_to_lower(str2, str1);

    assert(SA_strcmp(str2, "bonjour") == 0);

    SA_str_to_upper(str2, "bOnJour");

    assert(SA_strcmp(str2, "BONJOUR") == 0);

    SA_str_to_lower(str2, "");

    assert(SA_strcmp(str2, "") == 0);

    SA_str_to_lower(str2, "gightr");

    assert(SA_strcmp(SA_uint64_to_str(str2, 567), "567") == 0);
    assert(SA_strcmp(SA_uint64_to_str(str2, 0), "0") == 0);

    assert(SA_strcmp(SA_int64_to_str(str2, 567), "567") == 0);
    assert(SA_strcmp(SA_int64_to_str(str2, -567), "-567") == 0);
    assert(SA_strcmp(SA_int64_to_str(str2, -0), "0") == 0);
    assert(SA_strcmp(SA_int64_to_str(str2, -1), "-1") == 0);

    assert(SA_str_to_uint64("5678") == 5678);
    assert(SA_str_to_uint64("-5678") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_uint64("dfg") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_uint64("-") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_uint64("") == 0 && SA_get_last_error() == SA_ERROR_NAN);

    assert(SA_str_to_int64("5678") == 5678);
    assert(SA_str_to_int64("-5678") == -5678);
    assert(SA_str_to_int64("dfg") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_int64("-") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_int64("") == 0 && SA_get_last_error() == SA_ERROR_NAN);

    assert(SA_str_to_double("12.34") == 12.34);
    assert(SA_str_to_double(".34") == 0.34);
    assert(SA_str_to_double("3.3.4") == 0.0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_double(".") == 0.0);

    SA_strncpy(str2, "machin", 32);
    assert(SA_strcmp(str2, "machin") == 0);

    SA_strncpy(str2, "trucs", 4);
    assert(SA_strcmp(str2, "tru") == 0);

    SA_path_join(str2, 3, "machin", "chose", "bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    SA_path_join(str2, 3, "machin/", "chose/", "bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    assert(SA_startswith("bonjour", "bon") == SA_TRUE);
    assert(SA_startswith("bonjour", "fon") == SA_FALSE);
    assert(SA_startswith("bonjour", "bonjour") == SA_TRUE);
    assert(SA_startswith("bonjour", "bonjours") == SA_FALSE);
    assert(SA_startswith("bonjour", "BON") == SA_FALSE);

    assert(SA_startswith_case_unsensitive("bonjour", "bon") == SA_TRUE);
    assert(SA_startswith_case_unsensitive("bonjour", "fon") == SA_FALSE);
    assert(SA_startswith_case_unsensitive("bonjour", "bOnjoUr") == SA_TRUE);
    assert(SA_startswith_case_unsensitive("bonjour", "bonjours") == SA_FALSE);
    assert(SA_startswith_case_unsensitive("bonjour", "BON") == SA_TRUE);

    SA_strncpy(str2, "vive les gnus", 256);
    for(i = 0; str2[i] != '\0'; i++)
    {
        if(SA_parser_search_occurence_in_bytes_stream(str2[i], "les"))
        {
            assert(SA_strcmp(str2+i, "s gnus") == 0);
        }
    }

    assert(SA_int_pow(5, 7) == 78125);
    assert(SA_int_pow(5, 0) == 1);
    assert(SA_int_pow(0, 7) == 0);
    assert(SA_int_pow(0, 0) == 1);

    assert(SA_RELU(5) == 5);
    assert(SA_RELU(-5) == 0);
    assert(SA_RELU(6.7) == 6.7);

    for(int i = 0; i < 100; i++)
    {
        double x = SA_logistic(SA_random_unsecure_float(-1000, 1000));
        assert(x >= 0 && x <= 1.0);
    }
    for(int i = 0; i < 100; i++)
    {
        double x = SA_logistic(SA_random_unsecure_float(-10, 10));
        assert(x > 0 && x < 1.0);
    }

    SA_simplify_path(str2, "../.././.././././truc/chose/./bidule/./../machin/ttyy/eg/f/../../");
    assert(SA_strcmp(str2, "../../../truc/chose/machin/ttyy/") == 0);


    SA_microseconds t;
    int x = 0;

    t = SA_cpu_usage();
    for(register int i = 0; i < 1000000000; i++)
    {
        if(!(i>>1))
        {
            x++;
        }
    }
    printf("%lu\n", SA_cpu_usage() - t);

    t = SA_cpu_usage();
    for(register int i = 0; i < 1000000000; i++)
    {
        if(i == 0 || i == 1)
        {
            x++;
        }
    }
    printf("%lu\n", SA_cpu_usage() - t);

    /*
    ======================================
    ============ Test sockets ============
    ======================================
    */

    SA_SocketHandler* client_handler;
    char buffer[1024];
    char headers[] = "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:106.0) Gecko/20100101 Firefox/106.0\r\n"
        "Connection: close\r\n\r\n";
    
    client_handler = SA_socket_ssl_client_init("example.com", 443, NULL);

    if(client_handler == NULL)
    {
        SA_print_last_error();
        return 1;
    }

    SA_socket_send(client_handler, headers, SA_strlen(headers)+1, 0);

    while((i = SA_socket_recv(client_handler, buffer, sizeof(buffer), 0)) > 0)
    {
        buffer[i] = '\0';
        printf("%s", buffer);
    }

    SA_socket_close(&client_handler);  // this is very important because it will free all structures

    SA_destroy();
}
