#ifndef ATAG_C
#define ATAG_C

#include <kernel/atag.h>
#include <common/stdio.h>
#include <common/debug.h>

uint32_t get_mem_size(atag_t *tag) {
	if (tag->hdr.tag != CORE) {
		debug_puts("ATAG FAIL : no core tag\n");
		return 0;
	}
	while (tag->hdr.tag != NONE) {
		if (tag->hdr.tag == MEM) {
			return tag->mem.size;
		}
		tag = (atag_t *)(((uint32_t *)tag) + tag->hdr.size);
	}

	debug_puts("ATAG FAIL : no mem tag\n");
	return 0;
}

#endif
