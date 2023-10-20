#include "SA/parsing/parsing.h"

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