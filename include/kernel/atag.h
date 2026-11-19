#ifndef ATAG_H
#define ATAG_H

#include <stdint.h>

typedef enum {
	NONE		= 0x00000000,	/* Empty tag used to end list */
	CORE		= 0x54410001,	/* Start tag used to begin list */
	MEM			= 0x54410002,	/* Tag used to describe a physical area of memory */
	INITRD2		= 0x54420005,	/* Tag describing the physical location of the compressed ramdisk image */
	CMDLINE		= 0x54410009,	/* Tag used to pass the commandline to the kernel */
} atag_tag_t;

typedef struct {
	uint32_t size;
	atag_tag_t tag;
} atag_header_t;

typedef struct {
	uint32_t start;
	uint32_t size;
} atag_mem_t;

typedef struct {
	uint32_t start;
	uint32_t size;
} atag_initrd2_t;

typedef struct {
	char line[1];
} atag_cmdline_t;

typedef struct {
	atag_header_t hdr;
	union {
		atag_mem_t mem;
		atag_initrd2_t initrd2;
		atag_cmdline_t cmdline;
	};
} atag_t;

uint32_t get_mem_size(atag_t *atags);

#endif
