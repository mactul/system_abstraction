# MEMORY

- [MEMORY](#memory)
  - [Overview](#overview)
  - [Compilation](#compilation)
- [DynamicArray](#dynamicarray)
- [HashMap](#hashmap)
- [Memory Allocation](#memory-allocation)


## Overview

This section contains all functions related to memory interactions.

[dynamic_array](./dynamic_array.md) provides some functions to make an abstracted DynamicArray.
[hashmap](./hashmap.md) provides some functions to map key and values;
[mem_alloc](./mem_alloc.md) provides an overlay to malloc.

## Compilation

If you want SA to warn you when you forget to free some variables, you need to compile in debug mode.
```sh
# configure the compiler in debug mode
xmake f -m debug
# recompile all
xmake -r
# install the lib in /usr/local/ (you can specify -o if you want another destination)
xmake install
```

# DynamicArray

See the documentation [here](./dynamic_array.md)

# HashMap

See the documentation [here](./hashmap.md)

# Memory Allocation

See the documentation [here](./mem_alloc.md)