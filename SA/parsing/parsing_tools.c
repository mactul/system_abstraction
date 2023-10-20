#include "SA/parsing/parsing.h"
#include "SA/strings/strings.h"
#define MAX_URL_SIZE 1024

static int _buffer_index = 0;

/*
This function is used before `SA_parser_search_occurence_in_bytes_stream` to mark the incoming stream as new.
*/
void SA_parser_reset_stream(void)
{
    _buffer_index = 0;
}

/*
This function is used to search an occurence in a stream, byte per byte.
*/
SA_bool SA_parser_search_occurence_in_bytes_stream(char stream_single_byte, const char* occurence)
{
    if(stream_single_byte == occurence[_buffer_index])
    {
        _buffer_index++;
    }
    else
    {
        _buffer_index = 0;
    }
    if(occurence[_buffer_index] == '\0')
    {
        return SA_TRUE;
    }
    return SA_FALSE;
}
