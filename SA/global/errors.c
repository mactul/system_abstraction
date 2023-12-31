#include <stdio.h>
#include "SA/global/errors.h"


static int _SA_last_error = SA_NOERROR;

/*
Internal function to set the last error of the lib
*/
void _SA_set_error(int error_code)
{
    _SA_last_error = error_code;
}

/*
Returns the last error code of the lib
*/
enum SA_ERROR SA_get_last_error(void)
{
    return _SA_last_error;
}

/*
This is used to print a message on stderr
*/
void SA_print_error(const char* str)
{
    fputs(str, stderr);
}

/*
This is used in debug to display an user-friendly error message
*/
void SA_print_last_error(void)
{
    switch(_SA_last_error)
    {
        case SA_NOERROR:
            SA_print_error("no relevant error to print\n");
            break;
        case SA_ERROR_NAN:
            SA_print_error("Conversion failed, Not a Number\n");
            break;
        case SA_ERROR_MALLOC:
            SA_print_error("Allocation error !\nYou should close every non vital services\n");
            break;
        case SA_ERROR_SOCKET_ATTRIBUTION:
            SA_print_error("Socket attribution error\nUnix socket function returns -1, check errno\n");
            break;
        case SA_ERROR_CONNECTION_REFUSED:
            SA_print_error("Connection refused\nCheck your connection internet, check if the server is started\n");
            break;
        case SA_ERROR_UNABLE_TO_BIND:
            SA_print_error("Unable to bind\nMaybe the port is already taken. Use port=0 for an automatic attribution\n");
            break;
        case SA_ERROR_UNABLE_TO_LISTEN:
            SA_print_error("Unable to listen\n");
            break;
        case SA_ERROR_WRONG_PUBLIC_KEY_FP:
            SA_print_error("Wrong public key file path\nPlease provide a valid file path for your cert.pem file\n");
            break;
        case SA_ERROR_WRONG_PRIVATE_KEY_FP:
            SA_print_error("Wrong private key file path\nPlease provide a valid file path for your key.pem file\n");
            break;
        case SA_ERROR_SSL_CONNECTION_REFUSED:
            SA_print_error("SSL connection refused\n");
            break;
        case SA_ERROR_ACCEPT_FAILED:
            SA_print_error("Accept failed\n");
            break;
        case SA_ERROR_SSL_ACCEPT_FAILED:
            SA_print_error("SSL accept failed\n");
            break;
        case SA_ERROR_SSL_CTX_CREATION_FAILED:
            SA_print_error("SSL_CTX_new returns NULL, check the openssl documentation\n");
            break;
        case SA_ERROR_SSL_CREATION_FAILED:
            SA_print_error("SSL_new returns NULL, check the openssl documentation\n");
            break;
        case SA_ERROR_UNABLE_TO_FIND_ADDRESS:
            SA_print_error("There is no reachable IP address associated with this domain\n");
            break;
        case SA_ERROR_WRONG_PROTOCOL:
            SA_print_error("Wrong protocol, only http:// and https:// are currently supported\n");
            break;
        case SA_ERROR_WRITE_FAILED:
            SA_print_error("Unable to write to the stream\n");
            break;
        case SA_ERROR_UNABLE_TO_PARSE_DATA:
            SA_print_error("Unable to parse the response\n");
            break;
        case SA_ERROR_PORT_NUMBER_TO_BIG:
            SA_print_error("The port number specified is to big to fit in an uint16_t\n");
            break;
        case SA_ERROR_URL_SYNTAX:
            SA_print_error("The syntax of the url specified is wrong\n");
            break;
        case SA_ERROR_URL_TOO_LONG:
            SA_print_error("The url provided is too long, please update the #define SA_MAX_URI_LENGTH in SA/parsing/parsing.h\n");
            break;
        case SA_ERROR_OVERFLOW:
            SA_print_error("An overflow was detected\n");
            break;
        default:
            SA_print_error("Unknow error\n");
            break;
    }
}