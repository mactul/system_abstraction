#ifndef SA_STRINGS
    #define SA_STRINGS
    #include "SA/global/global.h"

    /**
     * @brief returns whether or not a character is between 'a' and 'z'
     * 
     * @param c the character to test
     * 
     * @note Warning, this macro has a side effect, c should not be an expression that modify the state of the program.
     */
    #define SA_CHAR_IS_LOWERCASE(c) ((c) >= 'a' && (c) <= 'z')

    /**
     * @brief returns whether or not a character is between 'A' and 'Z'
     * 
     * @param c the character to test
     * 
     * @note Warning, this macro has a side effect, c should not be an expression that modify the state of the program.
     */
    #define SA_CHAR_IS_UPPERCASE(c) ((c) >= 'A' && (c) <= 'Z')

    /**
     * @brief returns whether or not a character is between '0' and '9'
     * 
     * @param c the character to test
     * 
     * @note Warning, this macro has a side effect, c should not be an expression that modify the state of the program.
     */
    #define SA_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

    /**
     * @brief returns whether or not a character is an hexadecimal character
     * 
     * @param c the character to test
     * 
     * @note Warning, this macro has a side effect, c should not be an expression that modify the state of the program.
     */
    #define SA_CHAR_IS_HEXDIGIT(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Returns the number of characters in STR.  
     * @brief '\0' is not counted.
     * 
     * @param str the string to analyse
     * @return int
     */
    int SA_FUNCTION_PURE SA_strlen(const char* str);

    /**
     * @brief Returns whether or not the char `c` is in the string `str`
     * 
     * @param str the string to analyse
     * @param c the character we search
     * @return SA_bool 
     */
    SA_bool SA_FUNCTION_PURE SA_char_in_str(const char* str, char c);

    /**
     * @brief Returns whether or not the string STR is a unsigned number
     * 
     * @param str the string to analyse
     * @return SA_bool 
     */
    SA_bool SA_FUNCTION_PURE SA_is_uint(const char* str);

    /**
     * @brief Compare `str1` and `str2` with alphanumeric order.  
     * @brief This function is case dependend, use SA_strcasecmp if you want to be case independent  
     * 
     * @param str1
     * @param str2
     * 
     * @return - If str1 < str2, it will returns -1  
     * @return - If str1 > str2 it will returns 1  
     * @return - If str1 == str2, it will returns 0  
     */
    signed char SA_FUNCTION_PURE SA_strcmp(const char* str1, const char* str2);

    /**
     * @brief Compare `str1` and `str2` with alphanumeric order.  
     * @brief This function is case independend, use SA_strcmp if you want to be case dependent  
     * 
     * @param str1
     * @param str2
     * 
     * @return - If str1 < str2, it will returns -1  
     * @return - If str1 > str2 it will returns 1  
     * @return - If str1 == str2, it will returns 0  
     */
    signed char SA_FUNCTION_PURE SA_strcasecmp(const char* str1, const char* str2);

    /**
     * @brief Returns SA_TRUE if the first characters of `str` matches with `ref`.
     * 
     * @param str the string we want to check
     * @param ref the starting part we want to see in `str`
     * @return SA_bool
     * 
     * @note This function is case sensitive.
     */
    SA_bool SA_FUNCTION_PURE SA_startswith(const char* str, const char* ref);

    /**
     * @brief Returns SA_TRUE if the first characters of `str` matches with `ref`.
     * 
     * @param str the string we want to check
     * @param ref the starting part we want to see in `str`
     * @return SA_bool 
     * 
     * @note This function is case unsensitive.
     */
    SA_bool SA_FUNCTION_PURE SA_startswith_case_unsensitive(const char* str, const char* ref);

    /**
     * @brief Returns the position of the string `expr` in `str`  
     * @brief If `expr` is not in `str`, it returns -1
     * 
     * @param str
     * @param expr 
     * @return int
     * 
     * @note This function is case sensitive
     */
    int SA_FUNCTION_PURE SA_str_search(const char* str, const char* expr);

    /**
     * @brief Returns the position of the string `expr` in `str`  
     * @brief If `expr` is not in `str`, it returns -1
     * 
     * @param str 
     * @param expr 
     * @return int
     * 
     * @note This function is case unsensitive
     */
    int SA_FUNCTION_PURE SA_str_search_case_unsensitive(const char* str, const char* expr);

    /**
     * @brief Returns an equivalent in uppercase of the ascii character `c`.
     */
    char SA_FUNCTION_CONST SA_toupper(char c);

    /**
     * @brief Returns an equivalent in lowercase of the ascii character `c`.
     */
    char SA_FUNCTION_CONST SA_tolower(char c);

    /**
     * @brief Modify the string `str` and convert it to uppercase.  
     * @brief This only works for ascii characters.
     * 
     * @param str the string to convert
     */
    void SA_str_to_upper_inplace(char* str);

    /**
     * @brief Modify the string `str` and convert it to lowercase.  
     * @brief This only works for ascii characters.
     * 
     * @param str the string to convert
     */
    void SA_str_to_lower_inplace(char* str);

    /**
     * @brief Copy an uppercase version of the string `src` in `dest`.
     * 
     * @param dest the destination buffer should be at least the same size as `src`
     * @param src the string we want to convert
     */
    void SA_str_to_upper(char* dest, const char* src);

    /**
     * @brief Copy an lowercase version of the string `src` in `dest`.
     * 
     * @param dest the destination buffer should be at least the same size as `src`
     * @param src the string we want to convert
     */
    void SA_str_to_lower(char* dest, const char* src);

    /**
     * @brief Reverse the string, for example "hello" will be transformed to "olleh".  
     * @brief If you have no idea about the len of the string, you can use SA_strlen, like this:
     * @brief ```c
     * @brief reverse_string(str, SA_strlen(str));
     * @brief ```
     * 
     * @param str the string to reverse in place.
     * @param len_str the length of the string like provided by SA_strlen (often known when we want to reverse a string).
     */
    void SA_reverse_string(char* str, int len_str);

    /**
     * @brief Copy the content of `src` into `dest`  
     * @brief You can use it in cascade, like this:  
     * @brief ```c
     * @brief SA_strcpy(SA_strcpy(str, "hello"), " world");  // generate "hello world" in str
     * @brief ```
     * 
     * @param dest The destination buffer, should be at least the size of `src`
     * @param src The string we want to copy
     * @return A pointer to the end of the string, the `'\0'`
     */
    char* SA_strcpy(char* dest, const char* src);

    /**
     * @brief Copy the string `src` in `dest`.  
     * @brief If the size of `src` is bigger than `dest_buffer_size`, only the first bytes are copied.  
     * @brief In any case, `dest` is always terminated by '\0'.
     * 
     * @param dest The destination buffer
     * @param src The string to copy
     * @param dest_buffer_length The size of the buffer, should be `sizeof(dest)`
     * @return the number of characters copied, '\0' included
     */
    int SA_strncpy(char* dest, const char* src, int dest_buffer_size);

    /**
     * @brief Concatenate the string `add` to the end of `dest`.
     * @brief `dest` should be big enough to contains the two strings concatenated
     * 
     * @param dest the destination buffer containing de starting string
     * @param add the string to add at the end of dest
     */
    void SA_strcat(char* dest, const char* add);

    /**
     * @brief Set the `count` first bytes under `ptr` with `value`
     * 
     * @param ptr the buffer to set
     * @param value the value with which we want to set each byte
     * @param count the number of bytes to set
     * @return the pointer `ptr`
     */
    void* SA_memset(void* ptr, SA_byte value, uint64_t count);

    /**
     * @brief Set the `array_len` first uint32 under `dest` with `value`
     * 
     * @param dest the buffer to set
     * @param value the value with which we want to set each uint32
     * @param array_len the number of uin32 to set
     * @return the pointer `dest`
     */
    uint32_t* SA_memset32(uint32_t* dest, uint32_t value, size_t array_len);

    /**
     * @brief Copy the `size` first bytes of `source` into `dest`
     * 
     * @param dest the buffer in which we want to copy `source`
     * @param source the buffer to copy
     * @param size the size of `source` in bytes
     * @return the pointer `dest`
     */
    void* SA_memcpy(void* dest, void* source, uint64_t size);

    /**
     * @brief Modify the string `str` with whitespace stripped from the beginning and end of `str`  
     * @brief The characters removed are `' '`, `'\\t'`, `'\\b'`, `'\\n'`, `'\\r'` and `'\\v'`.
     * 
     * @param str the string to modify inplace.
     * @return the new beginning of the trimed string.
     */
    char* SA_WARN_UNUSED_RESULT SA_strtrim_inplace(char* str);

    /**
     * @brief Copy the string `src` in `dest` with whitespace stripped from the beginning and end of `src`  
     * @brief The characters removed are ' ', '\t', '\b', '\n', '\r' and '\v'.
     * 
     * @param dest destination buffer, must be at least as big as the length of `src`
     * @param src the source buffer
     */
    void SA_strtrim(char* dest, const char* src);

    /**
     * @brief Transform the signed integer n to a string and put it in result.  
     * @brief It also returns result this allows the function to be packed in another, like this:  
     * @brief ```c
     * @brief puts(SA_int64_to_str(result, 57));
     * @brief ```
     * 
     * @param result the buffer to contain the number
     * @param n the number to convert
     * @return the pointer `result`
     */
    char* SA_int64_to_str(char* result, int64_t n);

    /**
     * @brief Transform the unsigned integer n to a string and put it in result.  
     * @brief It also returns result this allows the function to be packed in another, like this:  
     * @brief ```c
     * @brief puts(SA_uint64_to_str(result, 57));
     * @brief ```
     * 
     * @param result the buffer to contain the number
     * @param n the number to convert
     * @return the pointer `result`
     */
    char* SA_uint64_to_str(char* result, uint64_t n);

    /**
     * @brief This transform a string to an uint64_t.  
     * @brief If the string is not the representation of an uint64_t, for example if it contains symbols like '-',
     * @brief the function will returns 0 and SA_get_last_error() will returns SA_ERROR_NAN.  
     * @brief Please make sure to check this if the function returns 0, unless you are sure that the string is well formated.
     * 
     * @param str the string to convert
     * @return uint64_t 
     */
    uint64_t SA_FUNCTION_PURE SA_str_to_uint64(const char* str);

    /**
     * @brief This transform a string to an int64_t.  
     * @brief If the string is not the representation of an int64_t, for example if it contains symbols like '.',
     * @brief the function will returns 0 and SA_get_last_error() will returns SA_ERROR_NAN.  
     * @brief Please make sure to check this if the function returns 0, unless you are sure that the string is well formated.
     * 
     * @param str the string to convert
     * @return int64_t 
     */
    int64_t SA_FUNCTION_PURE SA_str_to_int64(const char* str);

    /**
     * @brief This transform a string to a double.  
     * @brief If the string is not the representation of a double, for example if it contains symbols like '/',
     * @brief the function will returns 0.0 and SA_get_last_error() will returns SA_ERROR_NAN.  
     * @brief Please make sure to check this if the function returns 0.0, unless you are sure that the string is well formated.
     * 
     * @param str the string to convert
     * @return double
     */
    double SA_FUNCTION_PURE SA_str_to_double(const char* str);

    /**
     * @brief This transform a hexadecimal string to an uint64_t.
     * @brief If the string is not the hexadecimal representation of an uint64_t, for example if it contains symbols like '-',
     * @brief the function will returns 0 and SA_get_last_error() will returns SA_ERROR_NAN
     * @brief Please make sure to check this if the function returns 0, unless you are sure that the string is well formated.
     * 
     * @param str the string to convert
     * @return uint64_t
     */
    uint64_t SA_FUNCTION_PURE SA_hex_to_uint64(const char* str);

    #ifdef __cplusplus
    }
    #endif
#endif