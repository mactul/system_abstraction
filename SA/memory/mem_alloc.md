<- [MEMORY](./README.md)

- [Memory Allocation](#memory-allocation)
  - [__Functions__](#functions)
    - [SA\_malloc](#sa_malloc)
  - [__Examples__](#examples)


# Memory Allocation

intro

## __Functions__

### SA_malloc
```c
void* SA_malloc(size_t size)
```
- Allocate `size` bytes of memory.
- If SA is compiled in debug mode, it count the number of allocations to warn at the end of the program if something is not freed.
- **parameters:**
    - `size`: the number of bytes you want to allocate.
- **returns:**
    - when it succeeds, it returns a pointer to the memory allocated
    - when it fails, it returns `NULL` and you should 
- **example:**
    ```c
    SA_DynamicArray* dyn_array = SA_dynarray_create(int);
    ```


## __Examples__
