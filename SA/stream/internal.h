#ifndef SA_STREAM_INTERNAL
    #define SA_STREAM_INTERNAL

    #include "SA/stream/stream.h"

    struct _sa_stream_handler {
        SA_bool seek_back_authorized;
        void* stream;
        ssize_t (*reader)(void* stream, void* buffer, size_t n);
        ssize_t (*writer)(void* stream, const void* buffer, size_t n);
        SA_bool (*seeker)(void* stream, ssize_t pos);
        ssize_t (*teller)(const void* stream);
    };

#endif