# NETWORK

- [NETWORK](#network)
  - [Overview](#overview)
  - [Compilation](#compilation)
- [Easy\_TCP\_TLS](#easy_tcp_tls)
- [Requests](#requests)


## Overview

This section contains all functions related to network interactions.

[easy_tcp_tls](./easy_tcp_tls.md) provides some functions to easily make TCP socket; on top of an SSL layer or not.  
[requests](./requests.md) provides methods to make client HTTP(S) requests in a much easier way than libcurl.

## Compilation

If you don't want to use this portion of the library, you can compile with the option `build_network` set to `no` (default is `yes`)  
Here is an example:
```sh
# configure the compiler in debug mode, without the network
xmake f --build_network=n -m debug
# recompile all
xmake -r
# install the lib in /usr/local/ (you can specify -o if you want another destination)
xmake install
```
This operation will also remove the openssl dependance.


# Easy_TCP_TLS

See the documentation [here](./easy_tcp_tls.md)

# Requests

See the documentation [here](./requests.md)