#ifndef SA_STRINGS
    #define SA_STRINGS
    #include "SA/global/global.h"

    #define SA_CHAR_IS_LOWERCASE(c) ((c) >= 'a' && (c) <= 'z')
    #define SA_CHAR_IS_UPPERCASE(c) ((c) >= 'A' && (c) <= 'Z')

    #define SA_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
    #define SA_CHAR_IS_HEXDIGIT(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

    #ifdef __cplusplus
    extern "C"{
    #endif

    int SA_FUNCTION_PURE SA_strlen(const char* str);

    SA_bool SA_FUNCTION_PURE SA_char_in_str(const char* str, char c);

    SA_bool SA_FUNCTION_PURE SA_is_uint(const char* str);

    signed char SA_FUNCTION_PURE SA_strcmp(const char* str1, const char* str2);
    signed char SA_FUNCTION_PURE SA_strcasecmp(const char* str1, const char* str2);

    SA_bool SA_FUNCTION_PURE SA_startswith(const char* str, const char* ref);
    SA_bool SA_FUNCTION_PURE SA_startswith_case_unsensitive(const char* str, const char* ref);

    int SA_FUNCTION_PURE SA_str_search(const char* str, const char* expr);
    int SA_FUNCTION_PURE SA_str_search_case_unsensitive(const char* str, const char* expr);

    char SA_FUNCTION_CONST SA_toupper(char c);
    char SA_FUNCTION_CONST SA_tolower(char c);
    void SA_str_to_upper_inplace(char* str);
    void SA_str_to_lower_inplace(char* str);
    void SA_str_to_upper(char* dest, const char* src);
    void SA_str_to_lower(char* dest, const char* src);

    void SA_reverse_string(char* str, int len_str);

    char* SA_strcpy(char* dest, const char* src);
    int SA_strncpy(char* dest, const char* src, int n);
    void SA_strcat(char* dest, const char* add);

    void* SA_memset(void* ptr, char value, uint64_t count);
    uint32_t* SA_memset32(uint32_t* dest, uint32_t value, size_t array_len);
    void* SA_memcpy(void* dest, void* source, uint64_t size);

    char* SA_WARN_UNUSED_RESULT SA_strtrim_inplace(char* str);
    void SA_strtrim(char* dest, const char* src);

    char* SA_int64_to_str(char* result, int64_t n);
    char* SA_uint64_to_str(char* result, uint64_t n);
    uint64_t SA_FUNCTION_PURE SA_str_to_uint64(const char* str);
    int64_t SA_FUNCTION_PURE SA_str_to_int64(const char* str);
    double SA_FUNCTION_PURE SA_str_to_double(const char* str);

    uint64_t SA_FUNCTION_PURE SA_hex_to_uint64(const char* str);

    #ifdef __cplusplus
    }
    #endif
#endif