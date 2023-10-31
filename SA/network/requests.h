#ifndef SA_REQUESTS
    #define SA_REQUESTS
    #include "SA/global/global.h"

    typedef struct _SA_requests_handler SA_RequestsHandler;

    #ifdef __cplusplus
    extern "C"{
    #endif

    SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_request(SA_RequestsHandler* handler, const char* method, const char* url, const char* data, const char* additional_headers);
    const char* SA_req_get_header_value(SA_RequestsHandler* handler, const char* header_name);
    unsigned short int SA_req_get_status_code(SA_RequestsHandler* handler);
    void SA_req_display_headers(SA_RequestsHandler* handler);
    int SA_req_read_output_body(SA_RequestsHandler* handler, char* buffer, int buffer_size);
    void SA_req_close_connection(SA_RequestsHandler** ppr);

    #ifdef __cplusplus
    }
    #endif

    /* This part is all http methods implementation. */

    /*
    Performs a GET requests on the url URL.
    HANDLER should be NULL for the first request,
    then it should be the handler of a precedent request to perform keep-alive operations.
    ADDITIONAL_HEADERS can be "" or a list of headers separated by \r\n; in the case, the string must finish with \r\n.
    example: `"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"`
    */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_get(SA_RequestsHandler* handler, const char* url, const char* additional_headers)
    {
        return SA_req_request(handler, "GET ", url, "", additional_headers);
    }

    /*
    Performs a POST requests on the url URL.
    HANDLER should be NULL for the first request,
    then it should be the handler of a precedent request to perform keep-alive operations.
    ADDITIONAL_HEADERS can be "" or a list of headers separated by \r\n; in the case, the string must finish with \r\n.
    example: `"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"`
    DATA is the body of the requests, it can be for example `name=Jonathan&password=azerty1234`
    */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_post(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers)
    {
        return SA_req_request(handler, "POST ", url, data, additional_headers);
    }

    /*
    Performs a DELETE requests on the url URL.
    HANDLER should be NULL for the first request,
    then it should be the handler of a precedent request to perform keep-alive operations.
    ADDITIONAL_HEADERS can be "" or a list of headers separated by \r\n; in the case, the string must finish with \r\n.
    example: `"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"`
    */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_delete(SA_RequestsHandler* handler, const char* url, const char* additional_headers)
    {
        return SA_req_request(handler, "DELETE ", url, "", additional_headers);
    }

    /*
    Performs a PATCH requests on the url URL.
    HANDLER should be NULL for the first request,
    then it should be the handler of a precedent request to perform keep-alive operations.
    ADDITIONAL_HEADERS can be "" or a list of headers separated by \r\n; in the case, the string must finish with \r\n.
    example: `"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"`
    DATA is the body of the requests, it can be for example `name=Jonathan&password=azerty1234`
    */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_patch(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers)
    {
        return SA_req_request(handler, "PATCH ", url, data, additional_headers);
    }

    /*
    Performs a PUT requests on the url URL.
    HANDLER should be NULL for the first request,
    then it should be the handler of a precedent request to perform keep-alive operations.
    ADDITIONAL_HEADERS can be "" or a list of headers separated by \r\n; in the case, the string must finish with \r\n.
    example: `"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"`
    DATA is the body of the requests, it can be for example `name=Jonathan&password=azerty1234`
    */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_put(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers)
    {
        return SA_req_request(handler, "PUT ", url, data, additional_headers);
    }

    /*
    Performs a HEAD requests on the url URL.
    HANDLER should be NULL for the first request,
    then it should be the handler of a precedent request to perform keep-alive operations.
    ADDITIONAL_HEADERS can be "" or a list of headers separated by \r\n; in the case, the string must finish with \r\n.
    example: `"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"`
    */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_head(SA_RequestsHandler* handler, const char* url, const char* additional_headers)
    {
        return SA_req_request(handler, "HEAD ", url, "", additional_headers);
    }
#endif