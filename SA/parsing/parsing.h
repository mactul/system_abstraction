#ifndef SA_PARSING
    #define SA_PARSING
    #include "SA/global/global.h"
    #include "SA/parsing/parser_tree.h"

    #define SA_MAX_CHAR_ON_HOST 253  /* this is exact, don't change */
    #define SA_MAX_URI_LENGTH  1024 /* this can be changed, it's the maximum length a url can have */

    typedef struct _SA_url_splitted {
        char host[SA_MAX_CHAR_ON_HOST + 1];
        char uri[SA_MAX_URI_LENGTH + 1];
        uint16_t port;
        SA_bool secured;
    } SA_UrlSplitted;

    SA_bool SA_parse_url(const char* url, SA_UrlSplitted* url_splitted);

    #ifdef __cplusplus
    extern "C"{
    #endif
    
    void SA_parser_reset_stream(void);
    SA_bool SA_parser_search_occurence_in_bytes_stream(char stream_single_byte, const char* occurence);

    SA_ParserTree* SA_parse_urlencoded_form(const char* data);

    void SA_urldecode(char* dest, const char* src);

    #ifdef __cplusplus
    }
    #endif

    /*
    Decode an urlencoded string inplace (urlencoded string are equals or bigger than decoded ones).
    */
    static inline void SA_urldecode_inplace(char* data)
    {
        SA_urldecode(data, data);
    }

#endif