#ifndef SA_HASHMAP
    #define SA_HASHMAP
    #include "SA/global/global.h"

    typedef struct _SA_hashmap SA_HashMap;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Create a hashmap handler with the specified comparison function 
     * 
     * @param cmp_func A pointer to a callback which specify how to compare 2 keys. It should returns SA_TRUE if `*key1 == *key2`.
     * @return A handler to a SA_HashMap.
     */
    SA_HashMap* SA_WARN_UNUSED_RESULT SA_hashmap_create(SA_bool (*cmp_func)(void* key1, void* key2));

    /**
     * @brief Links the pointer `value` to the key under the pointer `key` in the HASHMAP.
     * If the key already have a value, it will overwrite the precedent value.
     * @param hashmap the handler returned by `SA_hashmap_create`
     * @param key a pointer to the key.
     * @param key_size the size of the key under the pointer KEY, in bytes. If the key is an uint64_t for example, KEY_SIZE should be 8.
     * @param value the value's pointer to store.
     * @return - SA_TRUE if it succeeded
     * - SA_FALSE if it fails to allocate the memory.
     */
    SA_bool SA_hashmap_set_value(SA_HashMap* hashmap, void* key, size_t key_size, void* value);

    /**
     * @brief Returns the pointer `value` associated with the value under `key` by `SA_hashmap_set_value`
     * 
     * @param hashmap the handler returned by `SA_hashmap_create`
     * @param key a pointer to the key.
     * @param key_size the size of the key under the pointer KEY, in bytes. If the key is an uint64_t for example, KEY_SIZE should be 8.
     * @return the pointer `value` if it was previously set, otherwise NULL.
     */
    void* SA_FUNCTION_PURE SA_hashmap_get_value(SA_HashMap* hashmap, void* key, size_t key_size);

    /**
     * @brief Free the underlying hashmap behind the pointer `hashmap` and set the hashmap handler to NULL.
     * 
     * @param hashmap the address of the handler returned by `SA_hashmap_create`
     */
    void SA_hashmap_free(SA_HashMap** hashmap);

    #ifdef __cplusplus
    }
    #endif
#endif