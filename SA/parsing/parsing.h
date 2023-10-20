#ifndef SA_PARSING
    #define SA_PARSING
    #include "SA/global/global.h"
    #include "SA/parsing/parser_tree.h"

    #ifdef __cplusplus
    extern "C"{
    #endif
    
    void SA_parser_reset_stream(void);
    SA_bool SA_parser_search_occurence_in_bytes_stream(char stream_single_byte, const char* occurence);

    SA_ParserTree* SA_parse_urlencoded_form(const char* data);

    #ifdef __cplusplus
    }
    #endif
#endif