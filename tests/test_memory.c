#include <stdio.h>
#include "test.h"


SA_bool cmp_int(void* int1, void* int2)
{
    return *((int*)int1) == *((int*)int2);
}


void test_memory2();

void test_memory(void)
{
    SA_DynamicArray* dyn_array;
    SA_HashMap* hashmap = SA_hashmap_create(cmp_int);
    int k1 = 56;
    int k2 = 56 << 16;

    int v1 = 578;
    int v2 = 23460;

    SA_hashmap_set_value(hashmap, &k1, sizeof(int), &v1);
    SA_hashmap_set_value(hashmap, &k2, sizeof(int), &v2);

    assert(SA_hashmap_get_value(hashmap, &k1, sizeof(int)) == &v1);
    assert(SA_hashmap_get_value(hashmap, &k2, sizeof(int)) == &v2);

    SA_hashmap_free(&hashmap);

    dyn_array = SA_dynarray_create(int);

    for(int i = 0; i < 534; i++)
    {
        SA_dynarray_append(int, dyn_array, i);
    }

    assert(SA_dynarray_get(int, dyn_array, 34) == 34);

    SA_dynarray_insert(int, dyn_array, 56, 3);

    SA_dynarray_set(int, dyn_array, 21, 1);

    assert(SA_dynarray_get(int, dyn_array, 21) == 1);

    assert(SA_dynarray_get(int, dyn_array, 34) == 34);
    assert(SA_dynarray_get(int, dyn_array, 56) == 3);

    SA_dynarray_insert(int, dyn_array, 12, 2);

    assert(SA_dynarray_get(int, dyn_array, 35) == 34);
    assert(SA_dynarray_get(int, dyn_array, 21) == 20);
    assert(SA_dynarray_get(int, dyn_array, 12) == 2);
    assert(SA_dynarray_get(int, dyn_array, 90) == 88);

    SA_dynarray_remove_block(dyn_array, 3, 2);

    assert(SA_dynarray_get(int, dyn_array, 90) == 90);

    SA_dynarray_remove_block(dyn_array, 3, 2000);

    assert(SA_dynarray_size(dyn_array) == 3);

    SA_dynarray_free(&dyn_array);

    test_memory2();
}

void test_memory2()
{
    SA_DynamicArray* dyn_array = SA_dynarray_create_size_hint(int, 10);

    SA_activate_zero_filling(dyn_array);

    SA_dynarray_set(int, dyn_array, 100, 45);

    return;
}