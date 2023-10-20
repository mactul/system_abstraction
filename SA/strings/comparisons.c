#include "SA/strings/strings.h"

/*
compare str1 and str2 with alphanumeric order.
This function is case dependend, use SA_strcasecmp if you want to be case independent
If str1 < str2, it will returns -1
If str1 > str2 it will returns 1
If str1 == str2, it will returns 0
*/
signed char SA_strcmp(const char* str1, const char* str2)
{
    while(*str1 != '\0' && *str2 != '\0' && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    if(*str1 == *str2)
    {
        return 0;
    }
    if(*str1 < *str2)
    {
        return -1;
    }
    return 1;
}

/*
compare str1 and str2 with alphanumeric order.
This function is case independend, use SA_strcmp if you want to be case dependent
If str1 < str2, it will returns -1
If str1 > str2 it will returns 1
If str1 == str2, it will returns 0
*/
signed char SA_strcasecmp(const char* str1, const char* str2)
{
    while(*str1 != '\0' && *str2 != '\0' && SA_tolower(*str1) == SA_tolower(*str2))
    {
        str1++;
        str2++;
    }
    if(SA_tolower(*str1) == SA_tolower(*str2))
    {
        return 0;
    }
    if(SA_tolower(*str1) < SA_tolower(*str2))
    {
        return -1;
    }
    return 1;
}

/*
Returns SA_TRUE if the first characters of STR matches with REF.
*/
SA_bool SA_startswith(const char* str, const char* ref)
{
    while(*str != '\0' && *ref != '\0' && *ref == *str)
    {
        str++;
        ref++;
    }
    return *ref == '\0';
}

/*
Returns SA_TRUE if the first characters of STR matches with REF.
This function is case unsensitive.
*/
SA_bool SA_startswith_case_unsensitive(const char* str, const char* ref)
{
    while(*str != '\0' && *ref != '\0' && SA_tolower(*ref) == SA_tolower(*str))
    {
        str++;
        ref++;
    }
    return *ref == '\0';
}