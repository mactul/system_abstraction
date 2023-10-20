#ifndef SA_PARSER_TREE
    #define SA_PARSER_TREE
    #include "SA/global/global.h"

    typedef struct _SA_parser_tree SA_ParserTree;

    #ifdef __cplusplus
    extern "C"{
    #endif

    SA_ParserTree* SA_ptree_init();
    SA_bool SA_ptree_update_key(SA_ParserTree* tree, const char* partial_key, int partial_key_len);
    SA_bool SA_ptree_update_value(SA_ParserTree* tree, const char* partial_value, int partial_value_len);
    SA_bool SA_ptree_push(SA_ParserTree* tree, void (*data_modifications)(char*));
    const char* SA_ptree_get_value(SA_ParserTree* tree, const char* key);
    void SA_ptree_free(SA_ParserTree** tree);
    void SA_ptree_abort(SA_ParserTree* tree);

    void SA_urldecode(char* dest, const char* src);
    void SA_urldecode_inplace(char* data);

    #ifdef __cplusplus
    }
    #endif
#endif