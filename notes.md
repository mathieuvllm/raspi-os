These are notes to myself, which I keep to remember things I've had trouble with.

# Address spaces
There are 3 address spaces (described in section 1.2.1-4 of BCM2835 datasheet) :
  - Virtual addresses : standard Linux kernel
  - Physical addresses : this is what we use in our code
  - Bus addresses : addresses described in the datasheet, must be translated into physical addresses

Peripheral bus address range starts at 0x7E000000, but physical address range starts at 0x3F000000 (0x20000000 for RPi 1). To translate bus into physical, subtract 0x3F000000 from the bus address.
