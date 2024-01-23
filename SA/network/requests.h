#ifndef SA_REQUESTS
    #define SA_REQUESTS
    #include "SA/global/global.h"

    typedef struct _SA_requests_handler SA_RequestsHandler;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief This is not meant to be used directly, unless you have exotic HTTP methods.  
     * @brief It's the generic method for all other HTTP methods.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param method This parameter must be in CAPS LOCK, followed by a space, like `"GET "`, `"POST "`, etc...
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param data It's the body of the request.
     * @param additional_headers The headers you want to specify, they are separeted by `\r\n` and __they needs__ to finish by `\r\n`.
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened.
     */
    SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_request(SA_RequestsHandler* handler, const char* method, const char* url, const char* data, const char* additional_headers);


    /**
     * @brief Get one of the parsed headers in the server response.
     * 
     * @param handler the handler returned by a request.
     * @param header_name the name of the header you want to get, for example `"content-length"` (this is case unsensitive).
     * @return - if the header exists in the server response, it will returns his value as a string.  
     * @return WARNING ! If you have to modify the string returned, copy it in a new buffer.
     * @return - if the headers is not returned by the server, it returns NULL.
     */
    const char* SA_FUNCTION_PURE SA_req_get_header_value(SA_RequestsHandler* handler, const char* header_name);


    /**
     * @brief This will return the server response code, you can usually check if this code is >= 400 to know if there is an error. (see http status code for more information)
     * 
     * @param handler the handler returned by a request.
     * @return an `unsigned short` which is the HTTP code
     */
    unsigned short int SA_FUNCTION_PURE SA_req_get_status_code(SA_RequestsHandler* handler);

    /**
     * @brief This is for debugging purpose, it will print the list of headers provided by the server.
     * 
     * @param handler the handler returned by a request.
     */
    void SA_req_display_headers(SA_RequestsHandler* handler);


    /**
     * @brief This function is what makes this library interesting alongside CURL.  
     * @brief It provides an interface that is like the usual socket read function.  
     * @brief This function skip the headers and recreate the body from chunks.  
     * @brief It's a blocking function, it waits for the data to arrive before transmitting them.  
     * @brief However, if there is no data left, you can call the function as many times as you want, it will never block you, it will just return 0 or a negative number if there is a problem.  
     * @brief This function fill the buffer entirely, excepted for the last one.
     * 
     * @param handler the handler returned by a request
     * @param buffer a buffer to fill with the data readed
     * @param buffer_size the size of the buffer. Don't forget to remove 1 byte from your real buffer size if you are reading text and you want to add an `'\0'` at the end of the buffer.
     * @return - If it succeded, it returns the number of bytes readed and put in `buffer`.
     * @return - If it fails, it returns -1 and errno contains more information.
     */
    int SA_req_read_output_body(SA_RequestsHandler* handler, char* buffer, int buffer_size);


    /**
     * @brief this function will close the connection, destroy the headers parsed tree, free all structures behind the handler and put your handler to `NULL`.
     * 
     * @param ppr the adress of your handler. It's a pointer to a pointer.
     */
    void SA_req_close_connection(SA_RequestsHandler** ppr);

    #ifdef __cplusplus
    }
    #endif

    /* This part is all http methods implementation. */

    /**
     * @brief send a GET request.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param additional_headers the headers you want to specify, they are separeted by `\\r\\n` and THEY NEED to finish by `\\r\\n`. example: `"Content-Type: application/json\\r\\nUser-Agent: requests.c\\r\\n"`
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened
     */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_get(SA_RequestsHandler* handler, const char* url, const char* additional_headers)
    {
        return SA_req_request(handler, "GET ", url, "", additional_headers);
    }


    /**
     * @brief send a POST request.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param data it's the body of the request, it can be for example `name=Jonathan&password=azerty1234`
     * @param additional_headers the headers you want to specify, they are separeted by `\\r\\n` and THEY NEED to finish by `\\r\\n`. example: `"Content-Type: application/json\\r\\nUser-Agent: requests.c\\r\\n"`
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened
     */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_post(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers)
    {
        return SA_req_request(handler, "POST ", url, data, additional_headers);
    }


    /**
     * @brief send a DELETE request.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param additional_headers the headers you want to specify, they are separeted by `\\r\\n` and THEY NEED to finish by `\\r\\n`. example: `"Content-Type: application/json\\r\\nUser-Agent: requests.c\\r\\n"`
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened
     */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_delete(SA_RequestsHandler* handler, const char* url, const char* additional_headers)
    {
        return SA_req_request(handler, "DELETE ", url, "", additional_headers);
    }

    /**
     * @brief send a POST request.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param data it's the body of the request, it can be for example `name=Jonathan&password=azerty1234`
     * @param additional_headers the headers you want to specify, they are separeted by `\\r\\n` and THEY NEED to finish by `\\r\\n`. example: `"Content-Type: application/json\\r\\nUser-Agent: requests.c\\r\\n"`
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened
     */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_patch(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers)
    {
        return SA_req_request(handler, "PATCH ", url, data, additional_headers);
    }

    /**
     * @brief send a PUT request.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param data it's the body of the request, it can be for example `name=Jonathan&password=azerty1234`
     * @param additional_headers the headers you want to specify, they are separeted by `\\r\\n` and THEY NEED to finish by `\\r\\n`. example: `"Content-Type: application/json\\r\\nUser-Agent: requests.c\\r\\n"`
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened
     */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_put(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers)
    {
        return SA_req_request(handler, "PUT ", url, data, additional_headers);
    }

    /**
     * @brief send a HEAD request.
     * 
     * @param handler must be NULL if it's the first connection, otherwise, it should be an old handler
     * @param url It's the url you want to request, it should start with `http://` or `https://`.
     * @param additional_headers the headers you want to specify, they are separeted by `\\r\\n` and THEY NEED to finish by `\\r\\n`. example: `"Content-Type: application/json\\r\\nUser-Agent: requests.c\\r\\n"`
     * @return - When it succededs, it returns a pointer to a structure handler.
     * @return - When it fails, it returns NULL and `SA_print_last_error` can tell what happened
     * 
     * @note Even if the server send a body response to this request (which is not possible in the HTTP standard), you will not be able to get it with SA_req_read_output_body.
     */
    static inline SA_RequestsHandler* SA_WARN_UNUSED_RESULT SA_req_head(SA_RequestsHandler* handler, const char* url, const char* additional_headers)
    {
        return SA_req_request(handler, "HEAD ", url, "", additional_headers);
    }
#endif