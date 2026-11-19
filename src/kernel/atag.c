#ifndef ATAG_C
#define ATAG_C

#include <kernel/atag.h>

uint32_t get_mem_size(atag_t *tag) {
	/* if this doesn't work, check tag->hdr.tag == CORE */
	while (tag->hdr.tag != NONE) {
		if (tag->hdr.tag == MEM)
			return tag->mem.size;
		tag = (atag_t *)(((uint32_t *)tag) + tag->hdr.size);
	}

	return 0;
}

#endif
