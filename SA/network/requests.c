#include <assert.h>
#include <stdio.h>
#include "SA/SA.h"

#define HEADERS_LENGTH   300  /* this is exact, don't change */

#define PARSER_BUFFER_SIZE 1024

struct _SA_requests_handler {
    SA_SocketHandler* handler;
    SA_ParserTree* headers_tree;
    int64_t total_bytes;
    uint64_t bytes_readed;
    char host[SA_MAX_CHAR_ON_HOST + 1];
    uint16_t port;
    char* reading_residue;
    int residue_size;
    int residue_offset;
    unsigned short int status_code;
    SA_bool read_finished;
    SA_bool chunked;
    SA_bool secured;
    char keep_alive_readed;
};

static int64_t min64(int64_t a, int64_t b)
{
    return a < b ? a: b;
}

static SA_bool req_parse_headers(SA_RequestsHandler* handler);
static int req_read_output(SA_RequestsHandler* handler, char* buffer, int n);
static SA_bool send_headers(SA_RequestsHandler* handler, char* headers);
static SA_bool connect_socket(SA_RequestsHandler* handler);


#ifdef DEBUG
static SA_bool is_first_call = SA_TRUE;
#endif


/*
This is not meant to be used directly, unless you have exotic HTTP methods.
*/
SA_RequestsHandler* SA_req_request(SA_RequestsHandler* handler, const char* method, const char* url, const char* data, const char* additional_headers)
{
    SA_UrlSplitted url_splitted;
    char content_length[30];
    char* headers = NULL;

    #ifdef DEBUG
    if(is_first_call && handler != NULL)
    {
        SA_print_error("DebugWarning: requests: handler pointer is non-null and it's the first time a request function is called\n"
                       "Make sure to initialize the handler to NULL before calling the first requests function.");
    }
    is_first_call = SA_FALSE;
    if(handler != NULL && (size_t)handler < 1000ULL)
    {
        SA_print_error("DebugWarning: requests: handler pointer is non-null and less than 1000, it may be uinitialized\n"
                       "Make sure to initialize the handler to NULL before calling the first requests function.");
    }
    #endif

    if(!SA_parse_url(url, &url_splitted))
    {
        return NULL;
    }

    SA_uint64_to_str(content_length, SA_strlen(data));
    

    // reserves the exact memory space for the request
    headers = (char*) SA_malloc((HEADERS_LENGTH + SA_strlen(method) + SA_strlen(url_splitted.uri) + SA_strlen(url_splitted.host) + SA_strlen(content_length) + SA_strlen(data) + SA_strlen(additional_headers))*sizeof(char));
    if(headers == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        SA_free(&handler);
        return NULL;
    }

    // build the request with all the datas
    SA_strcpy(headers, method);
    SA_strcat(headers, url_splitted.uri);
    SA_strcat(headers, " HTTP/1.1\r\nHost: ");
    SA_strcat(headers, url_splitted.host);
    SA_strcat(headers, "\r\nContent-Length: ");
    SA_strcat(headers, content_length);
    SA_strcat(headers, "\r\n");

    if(SA_str_search_case_unsensitive(additional_headers, "content-type:") == -1)  // we don't want to have the same header two times
    {
        SA_strcat(headers, "Content-Type: application/x-www-form-urlencoded\r\n");
    }
    if(SA_str_search_case_unsensitive(additional_headers, "accept") == -1)  // we don't want to have the same header two times
    {
        SA_strcat(headers, "Accept: */*\r\n");
    }
    if(SA_str_search_case_unsensitive(additional_headers, "user-agent") == -1)  // we don't want to have the same header two times
    {
        SA_strcat(headers, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36\r\n");
    }
    if(SA_str_search_case_unsensitive(additional_headers, "connection") == -1)  // we don't want to have the same header two times
    {
        SA_strcat(headers, "Connection: keep-alive\r\n");
    }
    if(SA_str_search_case_unsensitive(additional_headers, "accept-encoding") == -1)  // we don't want to have the same header two times
    {
        SA_strcat(headers, "Accept-Encoding: identity\r\n");
    }
    
    SA_strcat(headers, additional_headers);
    SA_strcat(headers, "\r\n");
    SA_strcat(headers, data);

    if(handler != NULL && SA_strcasecmp(handler->host, url_splitted.host) == 0 && handler->port == url_splitted.port && handler->secured == url_splitted.secured)
    {
        char trash_buffer[2048];
        //clean the socket
        while(SA_req_read_output_body(handler, trash_buffer, 2048) > 0)
        {
            ;
        }
        SA_ptree_free(&(handler->headers_tree));
        SA_free(&(handler->reading_residue));

        if(!send_headers(handler, headers) || SA_socket_recv(handler->handler, &(handler->keep_alive_readed), 1) <= 0)
        {
            SA_req_close_connection(&handler);  // connexion expired
        }
        // else: connexion successfuly reused
    }
    else if(handler != NULL)
    {
        SA_req_close_connection(&handler);
    }

    if(handler == NULL)
    {
        handler = (SA_RequestsHandler*) SA_malloc(sizeof(SA_RequestsHandler));
        if(handler == NULL)
        {
            SA_free(&headers);
            _SA_set_error(SA_ERROR_MALLOC);
            return NULL;
        }

        handler->keep_alive_readed = '\0';

        SA_strncpy(handler->host, url_splitted.host, SA_MAX_CHAR_ON_HOST+1);
        handler->port = url_splitted.port;
        handler->secured = url_splitted.secured;

        if(connect_socket(handler) == 0)
        {
            SA_free(&headers);
            SA_free(&handler);
            return NULL;
        }

        if(!send_headers(handler, headers))
        {
            _SA_set_error(SA_ERROR_WRITE_FAILED);
            SA_free(&headers);
            SA_req_close_connection(&handler);
            return NULL;
        }
    }

    SA_free(&headers);

    handler->headers_tree = NULL;
    handler->reading_residue = NULL;
    handler->residue_size = 0;
    handler->bytes_readed = 0;
    handler->residue_offset = 0;
    handler->read_finished = 0;
    handler->status_code = 0;

    if(!req_parse_headers(handler))
    {
        _SA_set_error(SA_ERROR_UNABLE_TO_PARSE_DATA);
        SA_req_close_connection(&handler);
        return NULL;
    }

    if(SA_strcmp(method, "HEAD ") == 0)
    {
        handler->read_finished = 1;
    }
    else
    {
        const char* response_content_length = SA_req_get_header_value(handler, "content-length");
        const char* transfer_encoding = SA_req_get_header_value(handler, "transfer-encoding");
        if(response_content_length == NULL || (transfer_encoding != NULL && SA_str_search_case_unsensitive(transfer_encoding, "chunked") != -1))
        {
            handler->total_bytes = 0;
            handler->chunked = 1;
        }
        else
        {
            handler->total_bytes = SA_str_to_uint64(response_content_length);
            if(handler->total_bytes == 0 && SA_get_last_error() == SA_ERROR_NAN)
            {
                _SA_set_error(SA_ERROR_UNABLE_TO_PARSE_DATA);
                SA_req_close_connection(&handler);
                return NULL;
            }
            handler->chunked = 0;
        }
    }

    const char* location = SA_ptree_get_value(handler->headers_tree, "location");
    if(location != NULL)
    {
        char temp_url[2*SA_MAX_URI_LENGTH];
        char location_url[2*SA_MAX_URI_LENGTH];
        char port_str[8] = "";
        char protocol[] = "https://";
        int n;
        if(SA_startswith(location, "http://") || SA_startswith(location, "https://"))
        {
            return SA_req_request(handler, method, location, data, additional_headers);
        }

        n = SA_strlen(url_splitted.uri);
        while(n > 0 && url_splitted.uri[n] != '/')
        {
            n--;
        }
        url_splitted.uri[n] = '\0';

        if((url_splitted.secured && url_splitted.port != 443) || (!url_splitted.secured && url_splitted.port != 80))
        {
            port_str[0] = ':';
            SA_uint64_to_str(port_str+1, url_splitted.port);
        }
        if(!url_splitted.secured)
        {
            SA_strcpy(protocol, "http://");
        }

        SA_strcpy(SA_strcpy(SA_strcpy(location_url, protocol), url_splitted.host), port_str);

        SA_path_join(temp_url, 2*SA_MAX_URI_LENGTH, 3, location_url, url_splitted.uri, location);
        SA_simplify_path(location_url, temp_url);
        return SA_req_request(handler, method, location_url, data, additional_headers);
    }

    return handler;
}

static unsigned short parse_status(char* key_value, char keep_alive_readed)
{   
    int k = 0;
    char status_code[4];
    int l = 0;

    if(!SA_startswith(key_value, "HTTP/") && (keep_alive_readed != 'H' || !SA_startswith(key_value, "TTP/")))
    {
        return 0;
    }

    // This is meant to happen with the first header "HTTP/1.1 ERROR_CODE MSG"

    while(key_value[k] != '\0' && key_value[k] != ' ')
        k++;
    if(key_value[k] != '\0')
        k++;
    while(l < 3 && SA_CHAR_IS_DIGIT(key_value[k]))
    {
        status_code[l] = key_value[k];
        l++;
        k++;
    }
    status_code[l] = '\0';
    return SA_str_to_uint64(status_code);
}


static SA_bool req_parse_headers(SA_RequestsHandler* handler)
{
    if(handler->headers_tree != NULL)
        return 1;
    
    char buffer[PARSER_BUFFER_SIZE];
    char key_value[PARSER_BUFFER_SIZE];

    SA_bool c_return = SA_FALSE;
    int offset = -1;
    int size;
    SA_bool in_value = SA_FALSE;
    int j = 0;

    handler->headers_tree = SA_ptree_init();

    while(offset == -1 && (size = req_read_output(handler, buffer, PARSER_BUFFER_SIZE)) > 0)
    {
        int i = 0;
        while(i < size && (buffer[i] != '\n' || !c_return))
        {
            key_value[j] = '\0';

            if(j == PARSER_BUFFER_SIZE-1)
            {
                char* trimed = SA_strtrim_inplace(key_value);
                if(in_value)
                {
                    if(SA_ptree_update_value(handler->headers_tree, trimed, SA_strlen(trimed)+1) == 0)
                        return SA_FALSE;
                }
                else
                {
                    if(SA_ptree_update_key(handler->headers_tree, trimed, SA_strlen(trimed)+1) == 0)
                        return SA_FALSE;
                }
                j = 0;
                key_value[0] = '\0';
            }
            if(!in_value && buffer[i] == ':')
            {
                in_value = SA_TRUE;
                char* trimed = SA_strtrim_inplace(key_value);
                if(SA_ptree_update_key(handler->headers_tree, trimed, SA_strlen(trimed)+1) == 0)
                    return SA_FALSE;
                j = 0;
            }
            else if(buffer[i] == '\n')
            {
                c_return = SA_TRUE;
                if(in_value)
                {
                    in_value = SA_FALSE;
                    char* trimed = SA_strtrim_inplace(key_value);
                    if(SA_ptree_update_value(handler->headers_tree, trimed, SA_strlen(trimed)+1) == 0)
                        return SA_FALSE;
                    if(SA_ptree_push(handler->headers_tree, NULL) == 0)
                        return SA_FALSE;
                }
                else
                {
                    unsigned short status_code = parse_status(key_value, handler->keep_alive_readed);
                    if(status_code != 0)
                    {
                        handler->status_code = status_code;
                    }
                    SA_ptree_abort(handler->headers_tree);
                }
                j = 0;
            }
            else
            {
                if(buffer[i] >= '0')
                    c_return = SA_FALSE;
                
                key_value[j] = buffer[i];
                j++;
            }

            i++;
        }
        if(i < size)
        {
            offset = i+1;
        }
    }
    
    handler->reading_residue = (char*) SA_malloc((size - offset) * sizeof(char));
    if(handler->reading_residue == NULL)
        return SA_FALSE;
    handler->residue_size = size - offset;
    SA_memcpy(handler->reading_residue, &(buffer[offset]), size-offset);

    return SA_TRUE;
}

/*
Returns the HTTP status code of the page
*/
unsigned short int SA_req_get_status_code(SA_RequestsHandler* handler)
{
    return handler->status_code;
}

/*
Returns the value of a specific header.
If HEADER_NAME is not in the server response headers, it returns NULL.
If you have to modify this value, copy it before.
*/
const char* SA_req_get_header_value(SA_RequestsHandler* handler, const char* header_name)
{
    return SA_ptree_get_value(handler->headers_tree, header_name);
}

/*
This display all headers in server response.
Used for debug purpose.
*/
void SA_req_display_headers(SA_RequestsHandler* handler)
{
    printf("STATUS CODE: %hu\n\n", handler->status_code);
    SA_ptree_display(handler->headers_tree);
}

static int64_t get_chunk_size(char c)
{
    static int i = 0;
    static char length[255];

    if(('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'))
    {
        length[i] = c;
        i++;
    }
    else if(c == '\r')
    {
        if(i == 0)
        {
            return -1;
        }
        length[i] = '\0';
        i++;
    }
    else if(c == '\n' && i != 0)
    {
        i = 0;
        return SA_hex_to_uint64(length);
    }
    return -1;
}

static int start_chunk_read(SA_RequestsHandler* handler, char* buffer, int bytes_in_buffer, int buffer_size)
{
    int offset = 0;
    handler->total_bytes = -1;
    while(handler->total_bytes == -1)
    {
        while(handler->total_bytes == -1 && offset < bytes_in_buffer)
        {
            handler->total_bytes = get_chunk_size(buffer[offset]);
            (offset)++;
        }
        if(handler->total_bytes == -1)
        {
            bytes_in_buffer = req_read_output(handler, buffer, buffer_size);
            offset = 0;
        }
    }
    
    if(handler->total_bytes == 0)
    {
        // That was the last one
        handler->read_finished = 1;
        return 0;
    }
    if(bytes_in_buffer <= 0)
    {
        return bytes_in_buffer;
    }
    bytes_in_buffer = SA_RELU(bytes_in_buffer - offset);
    SA_memcpy(buffer, &(buffer[offset]), bytes_in_buffer);
    handler->bytes_readed = bytes_in_buffer;

    
    if(bytes_in_buffer > handler->total_bytes)
    {
        int64_t chunk_size = handler->total_bytes;
        bytes_in_buffer = chunk_size + start_chunk_read(handler, &(buffer[handler->total_bytes]), bytes_in_buffer-chunk_size, buffer_size);
    }

    return bytes_in_buffer;
}


/*
    Skip the response header and fill the buffer with the server response
    Returns the number of bytes readed
    Use this in a loop.
    Note: This function reads binary datas.
    If you are getting text, use `sizeof(buffer)-1` for buffer_size and add an '\0' at the end of the readed buffer.
*/
int SA_req_read_output_body(SA_RequestsHandler* handler, char* buffer, int buffer_size)
{
    int size = 0;
    SA_bool new_chunk = SA_FALSE;

    assert(handler != NULL);

    if(handler->read_finished)
    {
        return 0;
    }
    if(handler->total_bytes > (int64_t)handler->bytes_readed)
    {
        int n = min64(buffer_size, handler->total_bytes - handler->bytes_readed);
        size = req_read_output(handler, buffer, n);
        if(size <= 0)
        {
            handler->read_finished = SA_TRUE;
            return 0;
        }
        handler->bytes_readed += size;
    }
    else if(handler->chunked)
    {
        new_chunk = SA_TRUE;
        size = req_read_output(handler, buffer, buffer_size);
        if(size <= 0)
        {
            handler->read_finished = SA_TRUE;
            return 0;
        }
    }

    if(size <= 0)
    {
        return size;
    }

    if(handler->chunked)
    {
        if(new_chunk)
        {
            size = start_chunk_read(handler, buffer, size, buffer_size);
        }
    }
    if(size < buffer_size)
    {
        return size + SA_req_read_output_body(handler, &(buffer[size]), buffer_size-size);
    }
    return size;
}

/*
    Fill the buffer with the http response
    Returns the numbers of bytes readed
    Can block if there is no data left.
*/
static int req_read_output(SA_RequestsHandler* handler, char* buffer, int n)
{
    int readed;
    if(handler->residue_size > 0)
    {
        readed = min64(n, handler->residue_size);
        SA_memcpy(buffer, &(handler->reading_residue[handler->residue_offset]), readed);
        handler->residue_size -= readed;
        handler->residue_offset += readed;
        if(handler->residue_size <= 0)
        {
            SA_free(&(handler->reading_residue));
        }
    }
    else
    {
        readed = SA_socket_recv(handler->handler, buffer, n);
    }

    return readed;
}

static SA_bool connect_socket(SA_RequestsHandler* handler)
{
    if(!handler->secured)
    {
        handler->handler = SA_socket_client_init(handler->host, handler->port);
        if(handler->handler == NULL)
        {
            return SA_FALSE;
        }
    }
    else
    {
        handler->handler = SA_socket_ssl_client_init(handler->host, handler->port);
        if(handler->handler == NULL)
        {
            return SA_FALSE;
        }
    }
    return SA_TRUE;
}

static SA_bool send_headers(SA_RequestsHandler* handler, char* headers)
{
    int total = SA_strlen(headers);
    int sent = 0;
    do {
        int bytes = SA_socket_send(handler->handler, headers+sent, total-sent);
        if (bytes < 0)
        {
            return SA_FALSE;
        }
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    return SA_TRUE;
}

/*
    Close the connection and free the ssl ctx.
    PPR must be the address of the socket handler.
*/
void SA_req_close_connection(SA_RequestsHandler** ppr)
{
    if(*ppr == NULL)
    {
        return;
    }
    SA_socket_close(&((*ppr)->handler));
    SA_ptree_free(&((*ppr)->headers_tree));
    SA_free(&((*ppr)->reading_residue));
    SA_free(ppr);
}