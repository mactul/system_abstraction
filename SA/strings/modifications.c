#include "SA/strings/strings.h"

/*
Returns an equivalent in uppercase of the ascii character C.
*/
char SA_toupper(char c)
{
    if(SA_CHAR_IS_LOWERCASE(c))
    {
        return c + ('A' - 'a');
    }
    return c;
}

/*
Returns an equivalent in lowercase of the ascii character C.
*/
char SA_tolower(char c)
{
    if(SA_CHAR_IS_UPPERCASE(c))
    {
        return c + ('a' - 'A');
    }
    return c;
}

/*
Modify the string STR and convert it to uppercase.
This only works for ascii characters.
*/
void SA_str_to_upper_inplace(char* str)
{
    while(*str != '\0')
    {
        if(SA_CHAR_IS_LOWERCASE(*str))
        {
            *str += ('A' - 'a');
        }
        str++;
    }
}

/*
Modify the string STR and convert it to lowercase.
This only works for ascii characters.
*/
void SA_str_to_lower_inplace(char* str)
{
    while(*str != '\0')
    {
        if(SA_CHAR_IS_UPPERCASE(*str))
        {
            *str += ('a' - 'A');
        }
        str++;
    }
}

/*
copy an uppercase version of the string SRC in DEST.
DEST should be the same size as SRC
*/
void SA_str_to_upper(char* dest, const char* src)
{
    while(*src != '\0')
    {
        *dest = SA_toupper(*src);
        src++;
        dest++;
    }
    *dest = '\0';
}

/*
copy an lowercase version of the string SRC in DEST.
DEST should be the same size as SRC
*/
void SA_str_to_lower(char* dest, const char* src)
{
    while(*src != '\0')
    {
        *dest = SA_tolower(*src);
        src++;
        dest++;
    }
    *dest = '\0';
}

/*
Copy the string SRC in DEST.
Returns the number of characters copied, '\0' included
If the length of SRC is bigger than DEST_BUFFER_LENGTH, only the first bytes are copied.
In any case, DEST is always terminated by '\0'.
*/
int SA_strncpy(char* dest, char* src, int dest_buffer_length)
{
    int i;
    for(i = 0; i < dest_buffer_length-1 && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
}

/*
Copy the content of SRC into DEST
DEST should be at least the size of SRC
return a pointer to the end of the string, the '\0'
*/
char* SA_strcpy(char* dest, char* src)
{
    while(*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return dest;
}

/*
Concatenate the string ADD to the end of DEST.
DEST should be big enough to contains the two strings concatenated
*/
void SA_strcat(char* dest, const char* add)
{
    while(*dest != '\0')
    {
        dest++;
    }
    while(*add != '\0')
    {
        *dest = *add;
        dest++;
        add++;
    }
    *dest = '\0';
}

/*
Modify the string STR with whitespace stripped from the beginning and end of STR
The characters removed are ' ', '\t', '\b', '\n', '\r' and '\v'.
The string returned is the new beginning of the trimed string.
*/
char* SA_strtrim_inplace(char* str)
{
    int i;
    while(SA_char_in_str(" \t\b\n\r\v", *str))
    {
        str++;
    }
    i = SA_strlen(str) - 1;
    while(SA_char_in_str(" \t\b\n\r\v", str[i]))
    {
        str[i] = '\0';
        i--;
    }

    return str;
}

/*
Copy the string STR in DEST with whitespace stripped from the beginning and end of STR
The characters removed are ' ', '\t', '\b', '\n', '\r' and '\v'.
The length of the DEST buffer must be at least as big as the length of STR
*/
void SA_strtrim(char* dest, const char* src)
{
    int i = 0;
    while(SA_char_in_str(" \t\b\n\r\v", *src))
    {
        src++;
    }
    while(src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while(SA_char_in_str(" \t\b\n\r\v", dest[i]))
    {
        dest[i] = '\0';
        i--;
    }
}

/*
Reverse the string, for example "hello" will be transformed to "olleh".
If you have no idea about the len of the string, you can use SA_strlen, like this:
```c
reverse_string(str, SA_strlen(str));
```
*/
void SA_reverse_string(char* str, int len_str)
{
    int i = 0;
    while(i < len_str)
    {
        char c = str[i];
        str[i] = str[len_str];
        str[len_str] = c;
        i++;
        len_str--;
    }
}

void* SA_memset(void* ptr, char value, uint64_t count)
{
    char* p = (char*)ptr;
    for(uint64_t i =0; i < count; i++)
    {
        p[i] = value;
    }
    return p;
}