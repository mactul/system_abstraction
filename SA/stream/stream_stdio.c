#include "SA/memory/mem_alloc.h"
#include "SA/stream/internal.h"

static inline ssize_t stdio_fread(void* stream, void* buffer, size_t n)
{
    n = fread(buffer, sizeof(char), n, (FILE*)stream);
    if(n == 0 && ferror((FILE*)stream) != 0)
    {
        return -1;
    }
    return n;
}

static inline ssize_t stdio_fwrite(void* stream, const void* buffer, size_t n)
{
    n = fwrite(buffer, sizeof(char), n, (FILE*)stream);
    if(ferror((FILE*)stream) != 0)
    {
        return -1;
    }
    return n;
}

static inline SA_bool stdio_fseek(void* stream, ssize_t pos)
{
    if(pos >= 0)
    {
        return fseeko((FILE*)stream, pos, SEEK_SET) == 0;
    }
    
    return fseeko((FILE*)stream, pos + 1, SEEK_END) == 0;
}

static inline ssize_t stdio_ftell(const void* stream)
{
    return ftello((FILE*)stream);
}

SA_StreamHandler* SA_stream_from_file(FILE* file)
{
    SA_StreamHandler* stream = (SA_StreamHandler*) SA_malloc(sizeof(SA_StreamHandler));
    if(stream == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return NULL;
    }

    stream->stream = (void*) file;
    stream->seek_back_authorized = SA_TRUE;
    stream->reader = stdio_fread;
    stream->writer = stdio_fwrite;
    stream->seeker = stdio_fseek;
    stream->teller = stdio_ftell;

    return stream;
}