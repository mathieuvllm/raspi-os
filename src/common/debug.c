#ifndef DEBUG_C
#define DEBUG_C

#include <common/debug.h>
#include <common/stdio.h>

void puthex(uint32_t v) {
	puts("0x");
	for (int i = 28; i >= 0; i -= 4) {
		putc("0123456789abcdef"[(v >> i) & 0xF]);
	}
}


#endif
