#include <ctype.h>
#include "SA/parsing/parsing.h"
#include "SA/strings/strings.h"
#define MAX_URL_SIZE 1024

static int _buffer_index = 0;

/*
This function is used before `SA_parser_search_occurrence_in_bytes_stream` to mark the incoming stream as new.
*/
void SA_parser_reset_stream(void)
{
    _buffer_index = 0;
}

/*
This function is used to search an occurrence in a stream, byte per byte.
*/
SA_bool SA_parser_search_occurrence_in_bytes_stream(char stream_single_byte, const char* occurrence)
{
    if(stream_single_byte == occurrence[_buffer_index])
    {
        _buffer_index++;
    }
    else
    {
        _buffer_index = 0;
    }
    if(occurrence[_buffer_index] == '\0')
    {
        return SA_TRUE;
    }
    return SA_FALSE;
}

/*
Decode an urlencoded string in SRC to the buffer DEST.
DEST should be at least the size of SRC (urlencoded string are equals or bigger than decoded ones).
*/
void SA_urldecode(char *dst, const char *src)
{
    char a, b;
    while (*src != '\0')
    {
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b)))
        {
            if (a >= 'a')
                a -= 'a' - 'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a' - 'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        }
        else if (*src == '+')
        {
            *dst++ = ' ';
            src++;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}
