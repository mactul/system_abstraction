<- [NETWORK](./README.md)

- [Requests](#requests)
  - [__Functions__](#functions)
    - [SA\_req\_get](#sa_req_get)
    - [SA\_req\_post](#sa_req_post)
    - [SA\_req\_delete](#sa_req_delete)
    - [SA\_req\_patch](#sa_req_patch)
    - [SA\_req\_put](#sa_req_put)
    - [SA\_req\_head](#sa_req_head)
    - [SA\_req\_request](#sa_req_request)
    - [SA\_req\_read\_output\_body](#sa_req_read_output_body)
    - [SA\_req\_close\_connection](#sa_req_close_connection)
    - [SA\_req\_get\_header\_value](#sa_req_get_header_value)
    - [SA\_req\_get\_status\_code](#sa_req_get_status_code)
    - [SA\_req\_display\_headers](#sa_req_display_headers)
  - [__Concepts__](#concepts)
    - [Url formating](#url-formating)
    - [Data formating](#data-formating)
    - [Headers formating](#headers-formating)
    - [Keep-alive](#keep-alive)
  - [__Examples__](#examples)
    - [Post - keep-alive disabled](#post---keep-alive-disabled)
    - [Get - Keep-alive enabled](#get---keep-alive-enabled)


# Requests

Requests is an easy to use library, influenced by requests.py, to make http requests


## __Functions__

### SA_req_get
```c
SA_RequestsHandler* SA_req_get(SA_RequestsHandler* handler, const char* url, const char* additional_headers);
```
- send a GET request.
- **parameters**
    - `handler`: must be NULL if it's the first connection, otherwise, it should be an old handler (see [keep-alive](#keep-alive) for more information).
    - `url`: It's the url you want to request, it should start with `http://` or `https://`. Currently, port number in the url isn't supported.
    - `additional_headers`: the headers you want to specify, they are separeted by `\r\n` and __they needs__ to finish by `\r\n`.
- **returns**
    - When it succededs, it returns a pointer to a structure handler.
    - When it fails, it returns NULL and `SA_print_last_error` can tell what happened

### SA_req_post
```c
SA_RequestsHandler* SA_req_post(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers);
```
- send a POST request.
- **parameters**
    - `handler`: must be NULL if it's the first connection, otherwise, it should be an old handler (see [keep-alive](#keep-alive) for more information).
    - `url`: It's the url you want to request, it should start with `http://` or `https://`. Currently, port number in the url isn't supported.
    - `data`: it's the body of the request.
    - `additional_headers`: the headers you want to specify, they are separeted by `\r\n` and __they needs__ to finish by `\r\n`.
- **returns**
    - When it succededs, it returns a pointer to a structure handler.
    - When it fails, it returns NULL and `SA_print_last_error` can tell what happened

### SA_req_delete
```c
SA_RequestsHandler* SA_req_delete(SA_RequestsHandler* handler, const char* url, const char* additional_headers);
```
- send a DELETE request.
- It works exactly like [SA_req_get](#sa_req_get).

### SA_req_patch
```c
SA_RequestsHandler* SA_req_patch(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers);
```
- send a PATCH request.
- It works exactly like [SA_req_post](#sa_req_post).

### SA_req_put
```c
SA_RequestsHandler* SA_req_put(SA_RequestsHandler* handler, const char* url, const char* data, const char* additional_headers);
```
- send a PUT request.
- It works exactly like [SA_req_post](#sa_req_post).

### SA_req_head
```c
SA_RequestsHandler* SA_req_head(SA_RequestsHandler* handler, const char* url, const char* additional_headers);
```
- send a HEAD request.
- Note: even if the server send a body response to this request (which is not possible in the HTTP standard), you will not be able to get it with [SA_req_read_output_body](#sa_req_read_output_body).
- For the rest, it works like [SA_req_get](#sa_req_get)

### SA_req_request
```c
SA_RequestsHandler* req_request(RequestsHandler* handler, const char* method, const char* url, const char* data, const char* additional_headers);
```
- **This is not meant to be used directly**, unless you have exotic HTTP methods.  
It's the generic method for all other HTTP methods.
- **parameters**
    - `handler`: must be NULL if it's the first connection, otherwise, it should be an old handler (see [keep-alive](#keep-alive) for more information).
    - `method`: This parameter must be in CAPS LOCK, followed by a space, like `"GET "`, `"POST "`, etc...
    - `url`: It's the url you want to request, it should start with `http://` or `https://`. Currently, port number in the url isn't supported.
    - `data`: it's the body of the request.
    - `additional_headers`: the headers you want to specify, they are separeted by `\r\n` and __they needs__ to finish by `\r\n`.
- **returns**
    - When it succededs, it returns a pointer to a structure handler.
    - When it fails, it returns NULL and `SA_print_last_error` can tell what happened


### SA_req_read_output_body
```c
int SA_req_read_output_body(SA_RequestsHandler* handler, char* buffer, int buffer_size);
```
- This function is what makes this library interesting alongside CURL.  
It provides an interface that is like the usual socket read function.  
This function skip the headers and recreate the body from chunks.
- It's a blocking function, it waits for the data to arrive before transmitting them.
However, if there is no data left, you can call the function as many times as you want, it will never block you, it will just return 0 or a negative number if there is a problem.
- This function fill the buffer entirely, excepted for the last one.
- **parameters**
  - `handler`: the handler returned by a request
  - `buffer`: a buffer to fill with the data readed
  - `buffer_size`: the size of the buffer. Don't forget to remove 1 byte from your real buffer size if you are reading text and you want to add an `'\0'` at the end of the buffer.
- **returns**:
    - If it succeded, it returns the number of bytes readed and put in `buffer`.
    - If it fails, it returns -1 and errno contains more information.


### SA_req_close_connection
```c
void SA_req_close_connection(SA_RequestsHandler** ppr);
```
- this function will close the connection, destroy the headers parsed tree, free all structures behind the handler and put your handler to `NULL`.
- **parameters**
  - `ppr`: the adress of your handler. It's a pointer to a pointer


### SA_req_get_header_value
```c
const char* SA_req_get_header_value(SA_RequestsHandler* handler, char* header_name);
```
- Get one of the parsed headers in the server response.
- **parameters**
    - `handler`: the handler returned by a request.
    - `header_name`: the name of the header you want to get, for example `"content-length"` (this is case unsensitive).
- **returns**
    - if the header exists in the server response, it will returns his value as a string.  
    **Warning !** If you have to modify the string returned, copy it in a new buffer.
    - if the headers is not returned by the server, it returns NULL.
- **example**:
    ```c
    const char* content_type = SA_req_get_header_value(handler, "content-type");
    ```
    Will set what the server returned as content-type in the content_type string, or NULL if the server does not provide this information.


### SA_req_get_status_code
```c
unsigned short int SA_req_get_status_code(SA_RequestsHandler* handler);
```
- This will return the server response code, you can usually check if this code is >= 400 to know if there is an error. (see http status code for more information)
- **parameters**
    - `handler`: the handler returned by a request.
- **returns**
    - an `unsigned short` which is the HTTP code


### SA_req_display_headers
- `void SA_req_display_headers(SA_RequestsHandler* handler);`
- This is for debugging purpose, it will print the list of headers provided by the server.
- **parameters**
    - `handler`: the handler returned by a request.


## __Concepts__

### Url formating

Requests is able to automatically parse urls, extract the domain and the relative uri.  
However, to work, url needs to start with `http://` or `https://`.  
Actually, you cannot set the port in the url (This will probably be the futur update).

### Data formating

If you send data (via post, put or patch), they need to be formated like the server want
for example, for a standard post *(application/x-www-form-urlencoded)*, it will be like that
```c
"key1=value1&key2=value2&key3=value3"
```

### Headers formating

You can always provide a empty string to the `additional_headers` parameter; However, if you provide any additional headers, they are separeted by `\r\n` and __they need__ to finish by `\r\n`\
for example, I can add this header
```c
"Content-Type: application/json\r\n"
```
Or these 2 headers:
```c
"Content-Type: application/json\r\nUser-Agent: requests.c\r\n"
```
If you forget the ending `\r\n` you will have a failure.


### Keep-alive
All the requests are keep-alive by default. However, if you provide NULL for the handler parameter in the request, you will never exploit this acceleration.
When you do multiple requests to the same server, you can provide the handler of the precedent request to the new one, to avoid retablishing the connection.

**/!\\ Warning /!\\** Make sure to always update the handler when you create connections with an old handler, like this:
```c
handler = SA_req_get(handler, ...)
```
If you don't do this, if the connection falls, your handler will be replaced by a new one that you'll never get back.  
*Note: on GNU systems, the compiler will warn you if you do that.*

To see a keep-alive example, see [Get - keep-alive enabled](#get---keep-alive-enabled).

## __Examples__

### Post - keep-alive disabled

An example of a post in `Connection: close`:
```c
#include <stdio.h>
#include "SA/SA.h"

int main()
{
    char buffer[1024];
    SA_RequestsHandler* handler;
    int size;

    SA_init();  // if you forget that, your code will never work on Windows and if you are in release mode, the socket creation will just fail without a warning
    
    handler = SA_req_post(NULL, "https://example.com", "user=MY_USER&password=MY_PASSWORD", "Connection: close\r\n");
    
    if(handler == NULL)
    {
        SA_print_last_error();
        goto PROGRAM_END;
    }
    
    SA_req_display_headers(handler);

    while((size = SA_req_read_output_body(handler, buffer, sizeof(buffer)-1)) > 0)
    {
        buffer[size] = '\0';
        printf("%s", buffer);
    }
    putchar('\n');

    SA_req_close_connection(&handler);

PROGRAM_END:
    SA_destroy();
}
```


### Get - Keep-alive enabled

This example download on the same host 3 times, then download another host (this will cause the handler to change).
```c
#include <stdio.h>
#include "SA/SA.h"

int main()
{
    char buffer[1024];
    SA_RequestsHandler* handler = NULL;  // It's very important to set the handler to NULL for the first connection, otherwise, you will have unexpected behaviors.
    int size;
    char* url_array[] = {
        "https://raw.githubusercontent.com/mactul/system_abstraction/main/SA/network/requests.c",
        "https://raw.githubusercontent.com/mactul/system_abstraction/main/SA/network/requests.h",
        "https://raw.githubusercontent.com/mactul/system_abstraction/main/SA/network/easy_tcp_tls.h",
        "https://example.com/test"
    };

    SA_init();  // if you forget that, your code will never work on Windows and if you are in release mode, the socket creation will just fail without a warning
    
    for(int i = 0; i < 4; i++)
    {
        handler = SA_req_get(handler, url_array[i], "");  // "" is for no additionals headers
        
        if(handler == NULL)
        {
            SA_print_last_error();
            goto PROGRAM_END;
        }
        
        SA_req_display_headers(handler);

        while((size = SA_req_read_output_body(handler, buffer, sizeof(buffer)-1)) > 0)
        {
            buffer[size] = '\0';
            printf("%s", buffer);
        }
        putchar('\n');
    }

PROGRAM_END:
    SA_req_close_connection(&handler);  // Please note that the close is only done for the last get. If you do it in the loop, the program will work, however, you will not benefit from the keep-alive speed improvement.

    SA_destroy();
}
```