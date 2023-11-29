#include <sys/stat.h>
#include "SA/memory/mem_alloc.h"
#include "SA/strings/strings.h"

void SA_recursive_mkdir(const char* path)
{
    int i;
    int j = 0;
    char* temp_path = (char*) SA_malloc((SA_strlen(path)+1)*sizeof(char));
    SA_strcpy(temp_path, path);

    while((i = SA_str_search(path+j, "/")) != -1)
    {
        if(j != 0)
        {
            temp_path[j-1] = '/';
        }
        j += i;
        temp_path[j] = '\0';
        mkdir(temp_path, 0755);
        j++;
    }

    if(j != 0)
    {
        temp_path[j-1] = '/';
    }

    mkdir(temp_path, 0755);

    SA_free(&temp_path);
}