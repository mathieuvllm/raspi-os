#ifndef ATAG_C
#define ATAG_C

#include <kernel/atag.h>

uint32_t get_mem_size(atag_t *atags) {
	/* if this doesn't work, check atags->hdr.tag == CORE */
	while (atags->hdr.tag != NONE) {
		if (atags->hdr.tag == MEM)
			return atags->mem.size;
		atags = ((uint32_t *)atags) + atags->hdr.size;
	}

	return 0;
}

#endif
