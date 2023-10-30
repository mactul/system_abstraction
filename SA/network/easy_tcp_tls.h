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

    void _SA_socket_start(void);
    void _SA_socket_cleanup(void);
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_ssl_server_init(const char* server_hostname, uint16_t server_port, int backlog, const char* public_key_fp, const char* private_key_fp);
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_ssl_client_init(const char* server_hostname, uint16_t server_port);
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_client_init(const char* server_hostname, uint16_t server_port);
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_server_init(const char* server_hostname, uint16_t server_port, int backlog);
    SA_SocketHandler* SA_WARN_UNUSED_RESULT SA_socket_accept(SA_SocketHandler* server, SA_ClientData* pclient_data);
    int SA_socket_send(SA_SocketHandler* s, const char* buffer, int n);
    int SA_socket_recv(SA_SocketHandler* s, char* buffer, int n);
    void SA_socket_close(SA_SocketHandler** pps);
    uint64_t SA_socket_ntoh64(uint64_t input);

    #ifdef __cplusplus
    }
    #endif
#endif