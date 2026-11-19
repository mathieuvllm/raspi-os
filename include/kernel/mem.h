#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <kernel/atag.h>

#define PAGE_SIZE 4096

typedef struct {
	uint8_t allocated: 1;		/* is this page allocated ? */
	uint8_t kernel_page: 1;		/* is this page part of the kernel? */
	uint32_t reserved: 30;
} page_flags_t;

typedef struct page {
	uint32_t vaddr_mapped;		/* virtual address that maps to this page */
	page_flags_t flags;
	struct page *next;
} page_t;

void mem_init(atag_t *atags);
void *alloc_page(void);
void free_page(void *ptr);

#endif
