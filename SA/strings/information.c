#include "SA/strings/strings.h"

/*
Returns the number of characters in STR.
'\0' is not counted.
*/
int SA_strlen(const char* str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
        ;
    return i;
}

/*
Returns whether or not the char C is in the string STR
*/
SA_bool SA_char_in_str(const char* str, char c)
{
    while(*str != '\0')
    {
        if(*str == c)
            return SA_TRUE;
        
        str++;
    }
    return SA_FALSE;
}

/*
Returns whether or not the string STR is a unsigned number
*/
SA_bool SA_is_uint(const char* str)
{
    while(*str != '\0')
    {
        if(!SA_CHAR_IS_DIGIT(*str))
        {
            return SA_FALSE;
        }
        str++;
    }
    return SA_TRUE;
}