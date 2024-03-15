#include "SA/memory/mem_alloc.h"
#include "SA/stream/internal.h"


SA_StreamHandler* SA_stream_create_empty(void* stream)
{
    SA_StreamHandler* handler = (SA_StreamHandler*) SA_calloc(sizeof(SA_StreamHandler));
    if(handler == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return NULL;
    }
    handler->stream = stream;

    return handler;
}

void SA_stream_set_reader(SA_StreamHandler* stream, ssize_t (*reader)(void* stream, void* buffer, size_t n))
{
    stream->reader = reader;
}

void SA_stream_set_writer(SA_StreamHandler* stream, ssize_t (*writer)(void* stream, const void* buffer, size_t n))
{
    stream->writer = writer;
}

SA_bool SA_stream_set_seeker(SA_StreamHandler* stream, SA_bool (*seeker)(void* stream, ssize_t pos), ssize_t (*teller)(const void* stream), SA_bool seek_back_authorized)
{
    if(teller == NULL || seeker == NULL)
    {
        return SA_FALSE;
    }

    stream->teller = teller;
    stream->seeker = seeker;
    stream->seek_back_authorized = seek_back_authorized;

    return SA_TRUE;
}

void SA_stream_set_seek_back_status(SA_StreamHandler* stream, SA_bool seek_back_authorized)
{
    stream->seek_back_authorized = seek_back_authorized;
}

ssize_t SA_stream_read(SA_StreamHandler* stream, void* buffer, size_t n)
{
    if(stream->reader == NULL)
    {
        return -1;
    }
    return stream->reader(stream->stream, buffer, n);
}

ssize_t SA_stream_write(SA_StreamHandler* stream, const void* buffer, size_t n)
{
    if(stream->writer == NULL)
    {
        return -1;
    }
    return stream->writer(stream->stream, buffer, n);
}

SA_bool SA_stream_seek(SA_StreamHandler* stream, ssize_t pos)
{
    if(stream->seeker == NULL || stream->teller == NULL)
    {
        return SA_FALSE;
    }
    if(!stream->seek_back_authorized && pos < stream->teller(stream->stream))
    {
        return SA_FALSE;
    }
    return stream->seeker(stream->stream, pos);
}

ssize_t SA_stream_tell(const SA_StreamHandler* stream)
{
    if(stream->teller == NULL)
    {
        return -1;
    }
    return stream->teller(stream->stream);
}