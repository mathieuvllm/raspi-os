#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

#define DEBUG 1

void debug_puts(char *str);

void puthex(uint32_t v);

#endif
