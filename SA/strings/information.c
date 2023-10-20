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
            return 1;
        
        str++;
    }
    return 0;
}