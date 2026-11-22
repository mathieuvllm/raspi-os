# Raspberry Pi kernel
This is a kernel / small OS I am developing for my Raspberry Pi 2 model B (revision 1.1).  
I am using jsandler18's [excellent tutorial](https://jsandler18.github.io/) as an inspiration, although my goal is to go beyond what is shown in the tutorial.  
This is still a WIP, as I have just finished implementing and testing memory pages.
## How to run the kernel
There are 2 ways to run the kernel :
  1. Using qemu
  2. Bare-metal (must have a Raspberry Pi 2)
### Using qemu
This is the simplest option right now. Simply run :
```sh
cd build && make run
```
### Bare-metal
WIP
## Makefile targets
The Makefile has 5 targets : `build`, `clean`, `run`, `debug`, and `test`.  
`build` creates the elf and img files of the kernel  
`clean` cleans the build folder  
`run` runs the kernel with qemu  
`debug` allows you to use gdb to debug the code  
`test` runs the tests I wrote
