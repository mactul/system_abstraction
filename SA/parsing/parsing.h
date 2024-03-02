#ifndef SA_PARSING
    #define SA_PARSING
    #include "SA/global/global.h"
    #include "SA/parsing/parser_tree.h"

    #define SA_MAX_CHAR_ON_HOST 253 /* this is exact, don't change */
    #define SA_MAX_URI_LENGTH 1024  /* this can be changed, it's the maximum length a url can have */

    typedef struct _SA_url_splitted
    {
        char host[SA_MAX_CHAR_ON_HOST + 1];
        char uri[SA_MAX_URI_LENGTH + 1];
        uint16_t port;
        SA_bool secured;
    } SA_UrlSplitted;

    #ifdef __cplusplus
    extern "C"
    {
    #endif


    /**
     * @brief split `url` into host, uri, port and whether or not it's http or https. Fill the structure `url_splitted` with these infos.
     *
     * @param url The url to split
     * @param url_splitted A pointer to SA_UrlSplitted structure that will be filled with the url components.
     * @return - When it succeeds, it returns SA_TRUE
     * @return - When it fails, it returns SA_FALSE and `SA_print_last_error()` can tell what happened.
     */
    SA_bool SA_parse_url(const char *url, SA_UrlSplitted *url_splitted);


    /**
     * @brief This is used in combination of `SA_parser_search_occurrence_in_bytes_stream`. Call this to specify that you are searching in new stream.
     *
     * @note This function is NOT thread safe.
     */
    void SA_parser_reset_stream(void);


    /**
     * @brief This function is used to search an occurrence in a stream, byte per byte.
     *
     * @param stream_single_byte The byte you are actually reading.
     * @param occurrence The string you are searching for.
     * @return - SA_FALSE if you have to continue searching.
     * @return - SA_TRUE if the string `occurrence` was encountered.
     *
     * @note This function is NOT thread safe.
     */
    SA_bool SA_parser_search_occurrence_in_bytes_stream(char stream_single_byte, const char *occurrence);


    /**
     * @brief This parse data encoded like `key1=value1&key2=value2&...`
     *
     * @param data the string to parse
     * @return A ParserTree which can be read by `SA_ptree...` functions.
     */
    SA_ParserTree *SA_parse_urlencoded_form(const char *data);

    
    /**
     * @brief Decode the urlencoded string in `src` to the buffer `dest`.
     * 
     * @param dest the destination string, should be at least the size of SRC (urlencoded string are equals or bigger than decoded ones).
     * @param src the source (encoded) string
     */
    void SA_urldecode(char *dest, const char *src);

    #ifdef __cplusplus
    }
    #endif


    /**
     * @brief Decode an urlencoded string inplace (urlencoded string are equals or bigger than decoded ones).
     * 
     * @param data the string to decode
     */
    static inline void SA_urldecode_inplace(char *data)
    {
        SA_urldecode(data, data);
    }

#endif