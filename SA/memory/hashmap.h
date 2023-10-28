#ifndef SA_HASHMAP
    #define SA_HASHMAP
    #include "SA/global/global.h"

    typedef struct _SA_hashmap SA_HashMap;

    #ifdef __cplusplus
    extern "C"{
    #endif

    SA_HashMap* SA_hashmap_create(SA_bool (*cmp_func)(void* str1, void* str2));
    SA_bool SA_hashmap_set_value(SA_HashMap* hashmap, void* key, int key_size, void* value);
    void* SA_hashmap_get_value(SA_HashMap* hashmap, void* key, int key_size);
    void SA_hashmap_free(SA_HashMap** hashmap);

    #ifdef __cplusplus
    }
    #endif
#endif