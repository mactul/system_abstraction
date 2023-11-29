# SystemAbstraction

## Installation

To install this library, you need xmake, you can download it here:
https://xmake.io/#/guide/installation

Then, you should run:
```sh
git clone https://github.com/mactul/system_abstraction/
cd system_abstraction
xmake f -m debug --build-tests=n
xmake -r
xmake install
```

You can also use the install.sh script instead of the 3 last commands:
```sh
git clone https://github.com/mactul/system_abstraction/
cd system_abstraction
sh install.sh
```

## Troubleshooting

One some systems, you may have to update your environment variables
You can edit the script `nano /etc/environment` or your bashrc/zshrc/etc...
Add these lines:
```sh
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
RPATH=$RPATH:/usr/local/lib
```
Then, restart your environment.


## Features
- [Memory](https://github.com/mactul/system_abstraction/tree/main/SA/memory)
- [Network](https://github.com/mactul/system_abstraction/tree/main/SA/network)
- Documentation in progress...