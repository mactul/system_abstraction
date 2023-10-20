#include "SA/strings/strings.h"

/*
Transform the unsigned integer n to a string and put it in result.
It also returns result this allows the function to be packed in another, like this:
```c
puts(SA_uint64_to_str(result, 57));
```
*/
char* SA_uint64_to_str(char* result, uint64_t n)
{
    int i = 0;
    do
    {
        result[i] = n % 10 + '0';
        i++;
    } while ((n /= 10) > 0);
    result[i] = '\0';
    i--;

    SA_reverse_string(result, i);

    return result;
}

/*
Transform the signed integer n to a string and put it in result.
It also returns result this allows the function to be packed in another, like this:
```c
puts(SA_int64_to_str(result, 57));
```
*/
char* SA_int64_to_str(char* result, int64_t n)
{
    int i = 0;
    char negative = 0;
    if(n < 0)
    {
        n *= -1;
        negative = 1;
        i++;
    }
    do
    {
        result[i] = n % 10 + '0';
        i++;
    } while ((n /= 10) > 0);
    result[i] = '\0';
    i--;

    SA_reverse_string(result+negative, i-negative);
    if(negative)
        result[0] = '-';

    return result;
}

/*
This transform a string to an uint64_t.
If the string is not the representation of an uint64_t, for example if it contains symbols like '-',
the function will returns 0 and SA_get_last_error() will returns SA_ERROR_NAN
Please make sure to check this if the function returns 0, unless you are sure that the string is well formated.
*/
uint64_t SA_str_to_uint64(const char* str)
{
    uint64_t result = 0;
    _SA_set_error(SA_NOERROR);
    do
    {
        if(!SA_CHAR_IS_DIGIT(*str))
        {
            _SA_set_error(SA_ERROR_NAN);
            return 0;
        }
        result *= 10;
        result += *str - '0';
        str++;
    } while(*str != '\0');
    return result;
}

/*
This transform a string to an int64_t.
If the string is not the representation of an int64_t, for example if it contains symbols like '.',
the function will returns 0 and SA_get_last_error() will returns SA_ERROR_NAN
Please make sure to check this if the function returns 0, unless you are sure that the string is well formated.
*/
int64_t SA_str_to_int64(const char* str)
{
    int64_t result = 0;
    int sign = 1;
    _SA_set_error(SA_NOERROR);

    if(*str == '-')
    {
        sign = -1;
        str++;
    }
    do
    {
        if(!SA_CHAR_IS_DIGIT(*str))
        {
            _SA_set_error(SA_ERROR_NAN);
            return 0;
        }
        result *= 10;
        result += *str - '0';
        str++;
    } while(*str != '\0');
    return result * sign;
}

/*
This transform a string to a double.
If the string is not the representation of a double, for example if it contains symbols like '/',
the function will returns 0.0 and SA_get_last_error() will returns SA_ERROR_NAN
Please make sure to check this if the function returns 0.0, unless you are sure that the string is well formated.
*/
double SA_str_to_double(const char* str)
{
    double result = 0.0;
    double decimal = 1.0;
    char dot = 0;
    double sign = 1;
    _SA_set_error(SA_NOERROR);

    if(*str == '-')
    {
        sign = -1;
        str++;
    }
    do
    {
        if(*str == '.' && !dot)
        {
            dot = 1;
            str++;
            continue;
        }
        if(!SA_CHAR_IS_DIGIT(*str))
        {
            _SA_set_error(SA_ERROR_NAN);
            return 0.0;
        }
        if(dot)
            decimal /= 10;
        else
            result *= 10;
        result += decimal * (*str - '0');
        str++;
    } while(*str != '\0');
    return result * sign;
}