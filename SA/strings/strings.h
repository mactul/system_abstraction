#ifndef SA_STRINGS
    #define SA_STRINGS
    #include "SA/global/global.h"

    #define SA_CHAR_IS_LOWERCASE(c) ((c) >= 'a' && (c) <= 'z')
    #define SA_CHAR_IS_UPPERCASE(c) ((c) >= 'A' && (c) <= 'Z')

    #define SA_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

    #ifdef __cplusplus
    extern "C"{
    #endif

    int SA_strlen(const char* str);

    SA_bool SA_char_in_str(const char* str, char c);

    signed char SA_strcmp(const char* str1, const char* str2);
    signed char SA_strcasecmp(const char* str1, const char* str2);

    SA_bool SA_startswith(const char* str, const char* ref);
    SA_bool SA_startswith_case_unsensitive(const char* str, const char* ref);

    char SA_toupper(char c);
    char SA_tolower(char c);
    void SA_str_to_upper_inplace(char* str);
    void SA_str_to_lower_inplace(char* str);
    void SA_str_to_upper(char* dest, const char* src);
    void SA_str_to_lower(char* dest, const char* src);

    void SA_reverse_string(char* str, int len_str);

    char* SA_strcpy(char* dest, char* src);
    int SA_strncpy(char* dest, char* src, int n);
    void SA_strcat(char* dest, const char* add);

    void* SA_memset(void* ptr, char value, uint64_t count);

    char* SA_strtrim_inplace(char* str);
    void SA_strtrim(char* dest, const char* src);

    char* SA_int64_to_str(char* result, int64_t n);
    char* SA_uint64_to_str(char* result, uint64_t n);
    uint64_t SA_str_to_uint64(const char* str);
    int64_t SA_str_to_int64(const char* str);
    double SA_str_to_double(const char* str);

    #ifdef __cplusplus
    }
    #endif
#endif