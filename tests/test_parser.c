#include "test.h"

void test_parser(void)
{
    SA_ParserTree* tree;
    char buffer[256];

    SA_strncpy(buffer, "vive les gnus", 256);
    SA_parser_reset_stream();
    for(int i = 0; buffer[i] != '\0'; i++)
    {
        if(SA_parser_search_occurrence_in_bytes_stream(buffer[i], "les"))
        {
            assert(SA_strcmp(buffer+i, "s gnus") == 0);
        }
    }

    tree = SA_parse_urlencoded_form("word=%C3%A9laborer&machin=truc&chose=bidule");
    assert(SA_strcmp("élaborer", SA_ptree_get_value(tree, "word")) == 0);
    assert(SA_strcmp("truc", SA_ptree_get_value(tree, "machin")) == 0);
    assert(SA_strcmp("bidule", SA_ptree_get_value(tree, "chose")) == 0);

    SA_ptree_free(&tree);
}