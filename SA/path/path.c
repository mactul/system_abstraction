#include <stdarg.h>
#include "SA/path/path.h"
#include "SA/strings/strings.h"


/*
Remove unwanted ../ and ./ in a path
For example, "abc/fgh/../ijk" => "abc/ijk"
DEST should be at least the size of SRC
*/
void SA_simplify_path(char* dest, const char* src)
{
    int i = 0;
    int j = 0;
    while(src[j] == '.' || src[j] == '/')
    {
        if(src[j] == '/' && src[j+1] == '.' && src[j+2] == '/')
        {
            j += 2;
        }
        else
        {
            dest[i] = src[j];
            i++;
            j++;
        }
    }
    while(src[j] != '\0')
    {
        dest[i] = src[j];

        if(i >= 4 && (dest[i] == '/' || dest[i] == '\0') && dest[i-1] == '.' && dest[i-2] == '.' && dest[i-3] == '/')
        {
            i -= 4;
            while(i >= 0 && dest[i] != '/')
            {
                i--;
            }
        }
        else if(i >= 3 && (dest[i] == '/' || dest[i] == '\0') && dest[i-1] == '.' && dest[i-2] == '/')
        {
            i -= 2;
        }
        i++;
        j++;
    }
    dest[i] = '\0';
}

/*
Concatenate each variadic element in PATH_DEST with '/' between each.
NB_ELEMENTS must contains the number of variadic parameters.
*/
void SA_path_join(char* path_dest, int nb_elements, ...)
{
    va_list args;
    va_start(args, nb_elements);

    for(int i =0; i < nb_elements; i++)
    {
        path_dest = SA_strcpy(path_dest, va_arg(args, char*));
        if(i != nb_elements - 1 && *(path_dest-1) != '/')
        {
            *path_dest = '/';
            path_dest++;
        }
    }
}