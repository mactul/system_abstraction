#ifndef SA_ERRORS
    #define SA_ERRORS
    
    /*
    This enum contains all possible errors that can be raised by the library.
    */
    enum SA_ERROR {
        SA_NOERROR,
        SA_ERROR_NAN,
        SA_ERROR_MALLOC,
        SA_ERROR_SOCKET_ATTRIBUTION,
        SA_ERROR_CONNECTION_REFUSED,
        SA_ERROR_UNABLE_TO_BIND,
        SA_ERROR_UNABLE_TO_LISTEN,
        SA_ERROR_WRONG_PUBLIC_KEY_FP,
        SA_ERROR_WRONG_PRIVATE_KEY_FP,
        SA_ERROR_SSL_CONNECTION_REFUSED,
        SA_ERROR_ACCEPT_FAILED,
        SA_ERROR_SSL_ACCEPT_FAILED,
        SA_ERROR_SSL_CTX_CREATION_FAILED,
        SA_ERROR_SSL_CREATION_FAILED,
        SA_ERROR_UNABLE_TO_FIND_ADDRESS,
        SA_ERROR_WRONG_PROTOCOL,
        SA_ERROR_WRITE_FAILED,
        SA_ERROR_UNABLE_TO_PARSE_RESPONSE
    };

    #ifdef __cplusplus
    extern "C"{
    #endif

    void _SA_set_error(int error_code);
    enum SA_ERROR SA_get_last_error(void);
    void SA_print_error(const char* str);
    void SA_print_last_error(void);

    #ifdef __cplusplus
    }
    #endif
#endif