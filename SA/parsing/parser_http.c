#include "SA/parsing/parsing.h"

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