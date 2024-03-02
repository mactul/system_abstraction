<- [NETWORK](./README.md)

- [Easy\_TCP\_TLS](#easy_tcp_tls)
  - [__Functions__](#functions)
    - [SA\_socket\_client\_init](#sa_socket_client_init)
    - [SA\_socket\_ssl\_client\_init](#sa_socket_ssl_client_init)
    - [SA\_socket\_server\_init](#sa_socket_server_init)
    - [SA\_socket\_ssl\_server\_init](#sa_socket_ssl_server_init)
    - [SA\_socket\_accept](#sa_socket_accept)
    - [SA\_socket\_send](#sa_socket_send)
    - [SA\_socket\_recv](#sa_socket_recv)
    - [SA\_socket\_close](#sa_socket_close)
  - [__Examples__](#examples)
    - [Basic client](#basic-client)
    - [Basic server](#basic-server)


# Easy_TCP_TLS

This library allows you to cipher all your code with ssl by only updating one line in your code.
For client applications, you just have to rename [SA_socket_client_init](#sa_socket_client_init) into [SA_socket_ssl_client_init](#sa_socket_ssl_client_init).
And for server applications, you have to rename [SA_socket_server_init](#sa_socket_server_init) into [SA_socket_ssl_server_init](#sa_socket_ssl_server_init) and add the `cert.pem` and `key.pem` arguments.

## __Functions__

### SA_socket_client_init
```c
SA_SocketHandler* SA_socket_client_init(const char* server_hostname, uint16_t server_port)
```
- This function will create the socket and returns a socket handler.
- **parameters:**
    - `server_hostname`: the targeted server host name, formatted like "127.0.0.1", like "2001:0db8:85a3:0000:0000:8a2e:0370:7334" or like "example.com"
    - `server_port`: the opened server port that listen the connection
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened

### SA_socket_ssl_client_init
```c
SA_SocketHandler* SA_socket_ssl_client_init(const char* server_hostname, uint16_t server_port)
```
- This function works like socket_client_init, but it will create an ssl secured socket connection.
- **parameters:**
    - `server_hostname`: the targeted server ip, formatted like "127.0.0.1", like "2001:0db8:85a3:0000:0000:8a2e:0370:7334" or like "example.com"
    - `server_port`: the opened server port that listen the connection
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened

### SA_socket_server_init
```c
SA_SocketHandler* SA_socket_server_init(const char* server_hostname, uint16_t server_port, int backlog)
```
- This function will create a server instance
- **parameters:**
    - `server_hostname`: this is always the hostname of your machine. For local applications it can be "127.0.0.1" or "localhost", in this case, the server will never listen to the outside world.
    - `server_port`: the port you want to listen. make sure it's not already taken or you will have an `SA_ERROR_UNABLE_TO_BIND` error. If your server isn't local, make sure your port is opened on your firewall.
    - `backlog`: The length of the connections queue before accept. It's just a hint for the compiler.
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened

### SA_socket_ssl_server_init
```c
SA_SocketHandler* SA_socket_ssl_server_init(const char* server_hostname, uint16_t server_port, int backlog, const char* public_key_fp, const char* private_key_fp)
```
- This function will create an ssl secured server instance
- You must generate a public and a private key with this command
    - `openssl req -x509 -newkey rsa:4096 -nodes -out ./cert.pem -keyout ./key.pem -days 365`
- **parameters:**
    - `server_hostname`: this is always the hostname of your machine. For local applications it can be "127.0.0.1" or "localhost", in this case, the server will never listen to the outside world.
    - `server_port`: the port you want to listen. make sure it's not already taken or you will have an `SA_ERROR_UNABLE_TO_BIND` error. If your server isn't local, make sure your port is opened on your firewall.
    - `backlog`: The length of the connections queue before accept. It's just a hint for the compiler.
    - `public_key_fp`: the path for your `cert.pem` file
    - `private_key_fp`: the path for your `key.pem` file
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened

### SA_socket_accept
```c
SA_SocketHandler* SA_socket_accept(SA_SocketHandler* server, SA_ClientData* pclient_data)
```
- This function is used in a server application to wait and accept client connections.
- **parameters:**
    - `server`: a pointer to a SA_SocketHandler, returned by - `SA_socket_server_init` or `SA_socket_ssl_server_init`
    - `pclient_data`: a pointer to a SA_ClientData structure. It can be NULL if you don't care about client ip infos.
        - SA_ClientData structure is defined like that:
            ```c
            typedef struct _SA_client_data {
                char ip[SA_ADDRSTRLEN];
                uint16_t port;
            } SA_ClientData;
            ```
        - If you have defined `SA_ClientData infos;`, you can access `infos.ip` and `infos.port`.
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `SA_socket_print_last_error()` can tell what happened

### SA_socket_send
```c
int SA_socket_send(SA_SocketHandler* s, const char* buffer, int n)
```
- This function will send the data contained in the buffer array through the socket
- **parameters:**
    - `s`: a pointer to a SocketHandler. If you are in a client application, it's the handler returned by `SA_socket_client_init` or `SA_socket_ssl_client_init`. If you are in a server application, it's the handler returned by `SA_socket_accept`
    - `buffer`: a buffer containing all the data you want to send
    - `n`: the size of the data, this can be different from the `sizeof(buffer)` if your buffer isn't full.
- **returns:**
    - when it succeeds, it returns the number of bytes sended
    - when it fails, it returns -1 and errno contains more information.

### SA_socket_recv
```c
int SA_socket_recv(SA_SocketHandler* s, char* buffer, int n)
```
- This function will wait for data to arrive in the socket and fill a buffer with them.
- **parameters:**
    - `s`: a pointer to a SocketHandler. If you are in a client application, it's the handler returned by `SA_socket_client_init` or `SA_socket_ssl_client_init`. If you are in a server application, it's the handler returned by `SA_socket_accept`.
    - `buffer`: an empty buffer that will be filled with data from the socket.
    - `n`: the size of your buffer, you can simply provide `sizeof(buffer)`.
- **returns:**
    - when it succeeds, it returns the number of bytes read
    - when it fails, it returns -1 and errno contains more information.

### SA_socket_close
```c
void SA_socket_close(SA_SocketHandler** pps)
```
- This function take the address of the pointer on the handler to release all the stuff and put the SA_SocketHandler pointer to NULL.
- **parameters:**
    - `pps`: the address of the pointer on the socket
- **returns:**
    - Nothing.
    - In theory, if the pointer pps passed is coherent, this function can't fail.
    - If it creates a runtime error, maybe the memory behind the pointer is not allocated properly.

## __Examples__

### Basic client

```c
#include "SA/SA.h"  // this will include everything from SA, but the only headers files you need are "SA/network/easy_tcp_tls.h" and "SA/strings/strings.h"

int main()
{
    SA_SocketHandler* client_handler;  // This handler represents the socket
    char buffer[100] = "hello world";

    SA_init();  // if you forget that, your code will never work on Windows and if you are in release mode, the socket creation will just fail without a warning

    client_handler = SA_socket_client_init("127.0.0.1", 3678);  // You can use SA_socket_ssl_client_init if your server require an ssl connection.

    if(client_handler == NULL)
    {
        SA_print_last_error();
        return 1;
    }

    SA_socket_send(client_handler, buffer, SA_strlen(buffer));  // We just send the relevant content of our buffer, the 11 first bytes.
    // We should verify the return value of socket_send, but it's just a basic example.

    SA_socket_close(&client_handler);  // this is very important because it will free all structures

    SA_destroy();
}
```

### Basic server
(It's the server corresponding with the client below).

```c
#include "SA/SA.h"  // this will include everything from SA, but the only headers files you need are "SA/network/easy_tcp_tls.h" and "SA/strings/strings.h"


int main()
{
    char buffer[1024];
    SA_SocketHandler* server_handler;

    SA_init();  // if you forget that, your code will never work on Windows and if you are in release mode, the socket creation will just fail without a warning
    
    server_handler = SA_socket_server_init("127.0.0.1", 3678, 1);  // You can use socket_ssl_server_init if you want the connection to be secure, but you have to create a cert.pem and a key.pem file.
    // Here we are using a backlog of 1, but you can (should ?) increase that if you want to handle multiple connections handshake in the same time (multiple established connections are independent from that).
    
    if(server_handler == NULL)
    {
        SA_print_last_error();
        return 1;
    }

    while(1)  // We loop to handle multiple connections (here, one at a time)
    {
        SA_SocketHandler* client_handler;
        SA_ClientData infos;
        int n;

        client_handler = SA_socket_accept(server_handler, &infos);  // We can also use NULL instead of &infos if we don't care about client infos
        if(client_handler == NULL)
        {
            SA_print_last_error();
            continue;  // we give up for this specific connection
        }

        printf("New client connected at:\n\tip: %s\n\tport: %d\n\n", infos.ip, infos.port);

        n = SA_socket_recv(client_handler, buffer, sizeof(buffer)-1);  // We use sizeof(buffer)-1 because we want a space to put an '\0'.
        if(n < 0)
        {
            SA_print_error("recv failed\n");
            continue;
        }
        buffer[n] = '\0';

        printf("Message received: \"%s\"\n", buffer);

        SA_socket_close(&client_handler);  // this is very important because it will free all structures
    }
    socket_close(&server_handler);  // We will never go here, but if wasn't a infinite loop, we must free the server handler at the end.

    SA_destroy();
}
```