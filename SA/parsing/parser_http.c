#include "SA/parsing/parsing.h"
#include "SA/strings/strings.h"

/*
This parse datas encoded like `key1=value1&key2=value2&...`
It returns a ParserTree which can be read by `SA_ptree...` functions.
*/
SA_ParserTree* SA_parse_urlencoded_form(const char* data)
{
    SA_ParserTree* tree = SA_ptree_init();
    while(*data != '\0')
    {
        int i = 0;
        while(data[i] != '\0' && data[i] != '=')
        {
            i++;
        }
        if(data[i] == '\0')
        {
            _SA_set_error(SA_ERROR_UNABLE_TO_PARSE_DATA);
            SA_ptree_abort(tree);
            SA_ptree_free(&tree);
            return NULL;
        }
        i++;
        
        SA_ptree_update_key(tree, data, i);
        data += i;
        i = 0;
        while(data[i] != '\0' && data[i] != '&')
        {
            i++;
        }
        SA_ptree_update_value(tree, data, i+1);
        if(data[i] != '\0')
        {
            i++;
        }
        data += i;

        SA_ptree_push(tree, SA_urldecode_inplace);
    }
    return tree;
}

SA_bool SA_parse_url(const char* url, SA_UrlSplitted* url_splitted)
{
    int i = 0;

    _SA_set_error(SA_NOERROR);

    if(SA_startswith(url, "https://"))
    {
        url_splitted->secured = SA_TRUE;
        url_splitted->port = 443;
        url += 8;
    }
    else if(SA_startswith(url, "http://"))
    {
        url_splitted->secured = SA_FALSE;
        url_splitted->port = 80;
        url += 7;
    }
    else
    {
        _SA_set_error(SA_ERROR_WRONG_PROTOCOL);
        return SA_FALSE;
    }


    // get the host from url
    i = 0;
    while(i < SA_MAX_CHAR_ON_HOST && *url != '\0' && *url != '/' && *url != '?' && *url != '#' && *url != ':')
    {
        url_splitted->host[i] = *url;
        i++;
        url++;
    }
    url_splitted->host[i] = '\0';

    // get the port if it is specified
    if(*url == ':')
    {
        char port_str[8];
        uint64_t temp_port;
        i = 0;
        url++;
        while(i < 7 && SA_CHAR_IS_DIGIT(*url))
        {
            port_str[i] = *url;
            i++;
            url++;
        }
        port_str[i] = '\0';
        temp_port = SA_str_to_uint64(port_str);
        if(temp_port > (uint16_t)(-1))
        {
            _SA_set_error(SA_ERROR_PORT_NUMBER_TO_BIG);
            return SA_FALSE;
        }
        if(temp_port != 0)
        {
            url_splitted->port = (uint16_t)temp_port;
        }
    }

    if(*url != '\0' && *url != '/' && *url != '?' && *url != '#' && *url != ':')
    {
        _SA_set_error(SA_ERROR_URL_SYNTAX);
        return SA_FALSE;
    }


    // get the relative url from url
    i = 0;
    while(i < SA_MAX_URI_LENGTH && *url != '\0' && *url != '#')
    {
        url_splitted->uri[i] = *url;
        i++;
        url++;
    }
    if(i == 0)
    {
        // There is no relative url
        url_splitted->uri[i] = '/';
        i++;
    }
    url_splitted->uri[i] = '\0';

    if(*url != '\0' && *url != '#')
    {
        _SA_set_error(SA_ERROR_URL_TOO_LONG);
        return SA_FALSE;
    }

    return SA_TRUE;
}