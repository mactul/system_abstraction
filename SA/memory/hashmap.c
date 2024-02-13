#include "SA/memory/hashmap.h"
#include "SA/memory/mem_alloc.h"

#define HASHSET_SIZE ((1 << 16)+1)


typedef struct _hashmap_node {
    void* key;
    void* value;
    struct _hashmap_node* next;
} HashmapNode;

struct _SA_hashmap {
    SA_bool (*cmp_func)(void* str1, void* str2);
    HashmapNode* nodes[HASHSET_SIZE];
};

static uint16_t hashmap_calc_hash(void* key, size_t size)
{
    uint16_t* k = (uint16_t*)key;
    uint16_t hash = 0;
    for(size_t i = 0; i < size/2; i++)
    {
        hash ^= *k;
        k++;
    }
    if(size & 0x1)
    {
        hash ^= *k;
    }
    return hash;
}

/*
Create a new hashmap.
The cmp_func parameter is a pointer to a function that specify how to compare 2 keys.
The cmp_function must returns SA_TRUE if key1 == key2 else SA_FALSE.
*/
SA_HashMap* SA_hashmap_create(SA_bool (*cmp_func)(void* key1, void* key2))
{
    SA_HashMap* hashmap = SA_calloc(sizeof(SA_HashMap));
    if(hashmap == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
    }
    hashmap->cmp_func = cmp_func;
    return hashmap;
}

/*
Links the pointer VALUE to the key under the pointer KEY in the HASHMAP.
KEY_SIZE is the size of the key under the pointer KEY, in bytes.
If the key is an uint64_t for example, KEY_SIZE should be 8.
*/
SA_bool SA_hashmap_set_value(SA_HashMap* hashmap, void* key, size_t key_size, void* value)
{
    uint16_t hash = hashmap_calc_hash(key, key_size);
    HashmapNode* last_node = hashmap->nodes[hash];
    HashmapNode* node;
    if(hashmap->nodes[hash] != NULL)
    {
        while(last_node->next != NULL && !hashmap->cmp_func(last_node->key, key))
        {
            last_node = last_node->next;
        }
        if(hashmap->cmp_func(last_node->key, key))
        {
            last_node->value = value;
            return SA_TRUE;
        }
    }
    node = (HashmapNode*) SA_malloc(sizeof(HashmapNode));
    if(node == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return SA_FALSE;
    }
    node->key = key;
    node->value = value;
    node->next = NULL;
    if(hashmap->nodes[hash] != NULL)
    {
        last_node->next = node;
    }
    else
    {
        hashmap->nodes[hash] = node;
    }
    return SA_TRUE;
}

/*
Returns the pointer VALUE associated with the key under KEY.
KEY_SIZE is the size of the key under the pointer KEY, in bytes.
If the key is an uint64_t for example, KEY_SIZE should be 8.

If the key isn't registered in the hashmap, then it returns NULL.
*/
void* SA_hashmap_get_value(SA_HashMap* hashmap, void* key, size_t key_size)
{
    uint16_t hash = hashmap_calc_hash(key, key_size);
    HashmapNode* node = hashmap->nodes[hash];
    while(node != NULL && !hashmap->cmp_func(node->key, key))
    {
        node = node->next;
    }
    if(node == NULL)
    {
        return NULL;
    }
    return node->value;
}

static void _free_list(HashmapNode* node)
{
    if(node == NULL)
    {
        return;
    }
    _free_list(node->next);
    SA_free(&node);
}

/*
Free the underlying hashmap behind the pointer HASHMAP and set the hashmap handler to NULL.
*/
void SA_hashmap_free(SA_HashMap** hashmap)
{
    if(*hashmap == NULL)
    {
        return;
    }
    for(int i = 0; i < HASHSET_SIZE; i++)
    {
        _free_list((*hashmap)->nodes[i]);
    }
    SA_free(hashmap);
}
