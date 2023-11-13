<- [MEMORY](./README.md)

- [DynamicArray](#dynamicarray)
  - [__Functions__](#functions)
    - [SA\_dynarray\_create](#sa_dynarray_create)
    - [SA\_dynarray\_append](#sa_dynarray_append)
    - [SA\_dynarray\_set](#sa_dynarray_set)
    - [SA\_dynarray\_insert](#sa_dynarray_insert)
    - [SA\_dynarray\_get](#sa_dynarray_get)
    - [SA\_dynarray\_insert\_uninitialized\_block](#sa_dynarray_insert_uninitialized_block)
    - [SA\_dynarray\_remove\_block](#sa_dynarray_remove_block)
    - [SA\_dynarray\_size](#sa_dynarray_size)
    - [SA\_dynarray\_get\_element\_size](#sa_dynarray_get_element_size)
    - [SA\_dynarray\_free](#sa_dynarray_free)
  - [__Examples__](#examples)


# DynamicArray

DynamicArray allows you to create an array which will automatically grow when you insert values into it.  
It uses a system of cast and macros to work with any type of value, a little bit like templates in C++.

This documentation is a little bit weird because I have to documentate macros and I wanted to specify the type of every function argument, but that's not always possible.  
Especially, when I write `type`, it refers to any type in C, like `int`, `float`, `char*`, or even types created by typedef `uint64_t`, `SA_SocketHandler*`, etc...

## __Functions__

### SA_dynarray_create
```c
SA_DynamicArray* SA_dynarray_create(type)
```
- This function will create a new DynamicArray for the type specified.
- **parameters:**
    - `type`: any type on which you can call `sizeof`, for example `int`, `float`, `struct vector`, etc... It will be the type used in the whole DynamicArray.
- **returns:**
    - when it succeeds, it returns a pointer to a structure handler.
    - when it fails, it returns `NULL` and `SA_print_last_error()` can tell what happened
- **example:**
    ```c
    SA_DynamicArray* dyn_array = SA_dynarray_create(int);
    ```


### SA_dynarray_append
```c
void SA_dynarray_append(type, SA_DynamicArray* dyn_array, type value)
```
- Append the value to the DynamicArray.
- **parameters:**
    - `type`: the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
    - `dyn_array`: the handler returned by `SA_dynarray_create`.
    - `value`: the value you want to append to the array, it's the type `type`
- **returns:**
    - It returns nothing, but it can fail by malloc error, in this case `SA_get_error` will return `SA_ERROR_MALLOC` or `SA_ERROR_OVERFLOW` (but this one is unlikely to happen)
- **example:**
    ```c
    SA_dynarray_append(int, dyn_array, 78);
    ```


### SA_dynarray_set
```c
void SA_dynarray_set(type, SA_DynamicArray* dyn_array, uint64_t index, type value)
```
- Set the value of a DynamicArray's column at the `index` position.
  If the index is bigger than the size of the array, the array is extended and the values between are filled with garbage.
- **parameters:**
    - `type`: the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
    - `dyn_array`: the handler returned by `SA_dynarray_create`.
    - `index`: the index of the column you want to set.
    - `value`: the value you want to set in the array, it's the type `type`.
- **returns:**
    - It returns nothing, but it can fail by malloc error, in this case `SA_get_error` will return `SA_ERROR_MALLOC` or `SA_ERROR_OVERFLOW` (but this one is unlikely to happen)
- **example:**
    ```c
    SA_dynarray_set(int, dyn_array, 12, 78);  // set the 13th element to 78
    ```


### SA_dynarray_insert
```c
void SA_dynarray_insert(type, SA_DynamicArray* dyn_array, uint64_t index, type value)
```
- Insert a value at the `index` position of a DynamicArray.
  If the index is bigger than the size of the array, the array is extended and the values between are filled with garbage.

  This is very costly, if you have multiple values to insert, use `SA_dynarray_insert_uinitiliazed_block` to create the space required.
  Then, use `SA_dynarray_set` to fill the space with your values.
- **parameters:**
    - `type`: the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
    - `dyn_array`: the handler returned by `SA_dynarray_create`.
    - `index`: the index of the column where you want to insert your value.
    - `value`: the value you want to insert in the array, it's the type `type`.
- **returns:**
    - It returns nothing, but it can fail by malloc error, in this case `SA_get_error` will return `SA_ERROR_MALLOC` or `SA_ERROR_OVERFLOW` (but this one is unlikely to happen)
- **example:**
    ```c
    SA_dynarray_insert(int, dyn_array, 12, 78);  // insert 78 between the 12th element and the 13th element.
    ```


### SA_dynarray_get
```c
type SA_dynarray_get(type, SA_DynamicArray* dyn_array, uint64_t index)
```
- Returns the value at position `index` in the array `dyn_array`.
- /!\\ WARNING /!\\ : If the index is bigger than the size of the array, you will have a segfault
- **parameters:**
    - `type`: the type specified in `SA_dynarray_create`, for example `int`, `float`, `struct vector`, etc...
    - `dyn_array`: the handler returned by `SA_dynarray_create`.
    - `index`: the index of the column you want to get.
- **returns:**
    - It returns the value of the array's column.
- **example:**
    ```c
    int x = SA_dynarray_get(int, dyn_array, 12);  // get the value of the 13th column of the array ant put it into the variable x.
    ```


### SA_dynarray_insert_uninitialized_block
```c
SA_bool SA_dynarray_insert_uninitialized_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements)
```
- This will move the dyn_array elements to create a space of uninitialized elements.  
  You should then set them using `SA_dynarray_set`.
- **parameters:**
    - `dyn_array`: the handler returned by `SA_dynarray_create`.
    - `index`: the index of the column where you want to insert the uninitialized block.
    - `nb_block_elements`: the number of elements in the uninitialized block.
- **returns:**
    - when it succeeds, it returns SA_TRUE;
    - when it fails, it returns SA_FALSE and `SA_print_last_error()` can tell what happened
- **example:**
    ```c
    if(!SA_dynarray_insert_uninitialized_block(dyn_array, 12, 7))  // insert 7 blocks after the 12th element.
    {
        // failure
        SA_print_last_error();
        exit(1);
    }
    for(int i = 0; i < 7; i++)
    {
        SA_dynarray_set(int, dyn_array, 12+i, 0);  // set each element to 0.
    }
    ```


### SA_dynarray_remove_block
```c
SA_bool SA_dynarray_remove_block(SA_DynamicArray* dyn_array, uint64_t index, uint64_t nb_block_elements)
```
- This will move the dyn_array elements to remove `nb_block_elements` at position `index`.
- **parameters:**
    - `dyn_array`: the handler returned by `SA_dynarray_create`.
    - `index`: the index of the column of the first element to remove.
    - `nb_block_elements`: the number of elements in the uninitialized block.
- **returns:**
    - when it succeeds, it returns SA_TRUE;
    - when it fails, it returns SA_FALSE. This can only happen if index is bigger than the size of the dyn_array.
- **example:**
    ```c
    SA_dynarray_remove_block(dyn_array, 12, 7);  // remove blocks from 13th to 20th
    ```


### SA_dynarray_size
```c
uint64_t SA_dynarray_size(SA_DynamicArray* dyn_array)
```
- Returns the number of elements in the DynamicArray


### SA_dynarray_get_element_size
```c
uint32_t SA_dynarray_get_element_size(SA_DynamicArray* dyn_array)
```
- Returns the size of the type used in the array.


### SA_dynarray_free
```c
void SA_dynarray_free(SA_DynamicArray** dyn_array)
```
- Free the underlying DynamicArray behind the pointer dyn_array and set the DynamicArray handler to NULL.
- **parameters**
    - `dyn_array`: the address of the pointer returned by `SA_dynarray_create`.

## __Examples__

```c
#include <stdio.h>
#include <SA/memory/dynamic_array.h>


int main()
{
    SA_DynamicArray* dyn_array = SA_dynarray_create(int);  // create an array of integers

    SA_init();

    for(int i = 0; i < 534; i++)  // create 534 elements
    {
        SA_dynarray_append(int, dyn_array, i);  // and their values are their index
    }

    printf("%d\n", SA_dynarray_get(int, dyn_array, 34));  // display the 35th element. will print 34

    SA_dynarray_insert(int, dyn_array, 56, 3);  // insert the value 3 at position 56

    SA_dynarray_set(int, dyn_array, 21, 1);  // set the 21st element to 1

    printf("%d\n", SA_dynarray_get(int, dyn_array, 21));  // display the 21st element, will print 1.

    printf("%d\n", SA_dynarray_get(int, dyn_array, 90));  // display the 91st element, will print 89 because everything is shifted by the insert.

    SA_dynarray_remove_block(dyn_array, 3, 2);  // will remove 2 elements at position 3.

    printf("%d\n", SA_dynarray_get(int, dyn_array, 90));  // display the 91st element, will print 91 because everything is shifted by the insert and the remove.

    SA_dynarray_free(&dyn_array);  // destroy the dyn_array.

    SA_destroy();
}
```