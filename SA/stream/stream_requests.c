#include "SA/memory/mem_alloc.h"
#include "SA/maths/maths.h"
#include "SA/stream/internal.h"

#define TRASH_BUFFER_SIZE 2048

static inline ssize_t requests_read(void* stream, void* buffer, size_t n)
{
    return SA_req_read_output_body((SA_RequestsHandler*)stream, buffer, n);
}

static inline SA_bool requests_seek(void* stream, ssize_t pos)
{
    ssize_t left;

    if(pos < 0)
    {
        return SA_FALSE;
    }
    
    while((left = pos - (ssize_t)SA_req_nb_bytes_read((SA_RequestsHandler*)stream)) > 0)
    {
        char buffer[TRASH_BUFFER_SIZE];
        if(left > TRASH_BUFFER_SIZE)
        {
            left = TRASH_BUFFER_SIZE;
        }
        if(SA_req_read_output_body((SA_RequestsHandler*)stream, buffer, left) == 0)
        {
            return SA_FALSE;
        }
    }
    return SA_TRUE;
}

static inline ssize_t requests_tell(const void* stream)
{
    return (ssize_t)SA_req_nb_bytes_read((SA_RequestsHandler*)stream);
}

SA_StreamHandler* SA_stream_from_requests(SA_RequestsHandler* request)
{
    SA_StreamHandler* stream = (SA_StreamHandler*) SA_malloc(sizeof(SA_StreamHandler));
    if(stream == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return NULL;
    }

    stream->stream = (void*) request;
    stream->seek_back_authorized = SA_FALSE;
    stream->reader = requests_read;
    stream->writer = NULL;
    stream->seeker = requests_seek;
    stream->teller = requests_tell;

    return stream;
}