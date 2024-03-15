#ifndef SA_STREAM
    #define SA_STREAM

    #define _LARGE_FILES 1
    #define _FILE_OFFSET_BITS 64
    #include <stdio.h>

    #include "SA/global/global.h"
    #include "SA/network/requests.h"

    typedef struct _sa_stream_handler SA_StreamHandler;

    #ifdef __cplusplus
    extern "C"{
    #endif

    SA_StreamHandler* SA_stream_create_empty(void* stream);
    void SA_stream_set_reader(SA_StreamHandler* stream, ssize_t (*reader)(void* stream, void* buffer, size_t n));
    void SA_stream_set_writer(SA_StreamHandler* stream, ssize_t (*writer)(void* stream, const void* buffer, size_t n));
    SA_bool SA_stream_set_seeker(SA_StreamHandler* stream, SA_bool (*seeker)(void* stream, ssize_t pos), ssize_t (*teller)(const void* stream), SA_bool seek_back_authorized);
    void SA_stream_set_seek_back_status(SA_StreamHandler* stream, SA_bool seek_back_authorized);

    ssize_t SA_stream_read(SA_StreamHandler* stream, void* buffer, size_t n);
    ssize_t SA_stream_write(SA_StreamHandler* stream, const void* buffer, size_t n);
    SA_bool SA_stream_seek(SA_StreamHandler* stream, ssize_t pos);
    ssize_t SA_stream_tell(const SA_StreamHandler* stream);


    SA_StreamHandler* SA_stream_from_file(FILE* file);
    SA_StreamHandler* SA_stream_from_requests(SA_RequestsHandler* request);

    #ifdef __cplusplus
    }
    #endif

    static inline void SA_stream_free(SA_StreamHandler** stream)
    {
        SA_free(stream);
    }
#endif