#ifndef SA_EASY_TCP_TLS
    #define SA_EASY_TCP_TLS
    #include "SA/global/global.h"

    #define SA_ADDRSTRLEN 22

    typedef struct _SA_client_data {
        char ip[SA_ADDRSTRLEN];
        uint16_t port;
    } SA_ClientData;

    typedef struct _SA_socket_handler SA_SocketHandler;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Internal function used to init the sockets for windows
     */
    void _SA_socket_start(void);

    /**
     * @brief Internal function that destroy all sockets on windows
     */
    void _SA_socket_cleanup(void);


    /**
     * @brief This function will create an ssl secured server instance
     * @brief You must generate a public and a private key with this command
     * @brief  - `openssl req -x509 -newkey rsa:4096 -nodes -out ./cert.pem -keyout ./key.pem -days 365`
     * 
     * @param server_hostname this is always the hostname of your machine. For local applications it can be "127.0.0.1" or "localhost", in this case, the server will never listen to the outside world.
     * @param server_port the port you want to listen. make sure it's not already taken or you will have an `SA_ERROR_UNABLE_TO_BIND` error. If your server isn't local, make sure your port is opened on your firewall.
     * @param backlog The length of the connections queue before accept. It's just a hint for the compiler.
     * @param public_key_fp the path for your `cert.pem` file
     * @param private_key_fp the path for your `key.pem` file
     * @return - when it succeeds, it returns a pointer to a structure handler.
     * @return - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
     */
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_ssl_server_init(const char* server_hostname, uint16_t server_port, int backlog, const char* public_key_fp, const char* private_key_fp);


    /**
     * @brief This function works like socket_client_init, but it will create an ssl secured socket connexion.
     * 
     * @param server_hostname the targeted server ip, formated like "127.0.0.1", like "2001:0db8:85a3:0000:0000:8a2e:0370:7334" or like "example.com"
     * @param server_port the opened server port that listen the connection
     * @return - when it succeeds, it returns a pointer to a structure handler.
     * @return - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
     */
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_ssl_client_init(const char* server_hostname, uint16_t server_port);


    /**
     * @brief This function will create the socket and returns a socket handler.
     * 
     * @param server_hostname the targeted server host name, formated like "127.0.0.1", like "2001:0db8:85a3:0000:0000:8a2e:0370:7334" or like "example.com"
     * @param server_port the opened server port that listen the connection
     * @return - when it succeeds, it returns a pointer to a structure handler.
     * @return - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
     */
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_client_init(const char* server_hostname, uint16_t server_port);


    /**
     * @brief This function will create a server instance
     * 
     * @param server_hostname this is always the hostname of your machine. For local applications it can be "127.0.0.1" or "localhost", in this case, the server will never listen to the outside world.
     * @param server_port the port you want to listen. make sure it's not already taken or you will have an `SA_ERROR_UNABLE_TO_BIND` error. If your server isn't local, make sure your port is opened on your firewall.
     * @param backlog The length of the connections queue before accept. It's just a hint for the compiler.
     * @return - when it succeeds, it returns a pointer to a structure handler.
     * @return - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
     */
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_server_init(const char* server_hostname, uint16_t server_port, int backlog);


    /**
     * @brief This function is used in a server application to wait and accept client connections.
     * 
     * @param server a pointer to a SA_SocketHandler, returned by `SA_socket_server_init` or `SA_socket_ssl_server_init`
     * @param pclient_data a pointer to a SA_ClientData structure. It can be NULL if you don't care about client ip infos.
     * @return - when it succeeds, it returns a pointer to a structure handler.
     * @return - when it fails, it returns `NULL` and `SA_socket_print_last_error()` can tell what happened
     */
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_accept(SA_SocketHandler* server, SA_ClientData* pclient_data);


    /**
     * @brief This function will send the datas contained in the buffer array through the socket
     * 
     * @param s a pointer to a SocketHandler. If you are in a client application, it's the handler returned by `SA_socket_client_init` or `SA_socket_ssl_client_init`. If you are in a server application, it's the handler returned by `SA_socket_accept`
     * @param buffer a buffer containing all the datas you want to send
     * @param n the size of the datas, this can be different from the `sizeof(buffer)` if your buffer isn't full.
     * @return - when it succeeds, it returns the number of bytes sended
     * @return - when it fails, it returns -1 and errno contains more information.
     */
    int SA_socket_send(SA_SocketHandler* s, const char* buffer, int n);

    /**
     * @brief This function will wait for data to arrive in the socket and fill a buffer with them.
     * 
     * @param s a pointer to a SocketHandler. If you are in a client application, it's the handler returned by `SA_socket_client_init` or `SA_socket_ssl_client_init`. If you are in a server application, it's the handler returned by `SA_socket_accept`.
     * @param buffer an empty buffer that will be filled with data from the socket.
     * @param n the size of your buffer, you can simply provide `sizeof(buffer)`.
     * @return - when it succeeds, it returns the number of bytes readed
     * @return - when it fails, it returns -1 and errno contains more information.
     */
    int SA_socket_recv(SA_SocketHandler* s, char* buffer, int n);


    /**
     * @brief This function take the address of the pointer on the handler to release all the stuff and put the SA_Sockethandler pointer to NULL.
     * 
     * @param pps the address of the pointer on the socket
     */
    void SA_socket_close(SA_SocketHandler** pps);


    /**
     * @brief Change a number from little endian to big endian or from big endian to little endian but only if the machine is little-endian. Otherwise, it do nothing.
     * 
     * @param input the number to convert
     * @return the number converted
     */
    uint64_t SA_FUNCTION_CONST SA_socket_ntoh64(uint64_t input);

    #ifdef __cplusplus
    }
    #endif
#endif