#ifndef MEM_C
#define MEM_C

#include <kernel/mem.h>
#include <kernel/atag.h>
#include <common/stdlib.h>
#include <stdint.h>
#include <common/debug.h>
#include <common/stdio.h>

extern uint8_t __end;	/* symbol declared in build/linker.ld */

static uint32_t num_pages;

static void check_page(void *p);

typedef struct page_list {
	page_t *head;
	uint32_t size;
} page_list_t;

void append_page_list(page_list_t *list, page_t *node) {  
	node->next = nullptr;
	list->size += 1;
	if (!list->head)
		list->head = node;
}

void push_page_list(page_list_t *list, page_t *node) { 
	node->next = list->head;
	list->head = node;
	list->size += 1;
}

page_t *peek_page_list(page_list_t *list) {
	return list->head;
}

page_t *pop_page_list(page_list_t *list) { 
	page_t *res = list->head;

	if (res) {
		list->head = list->head->next;
		list->size -= 1;
	}

	return res;
}

uint32_t size_page_list(page_list_t *list) {
	return list->size;
}

page_t *next_page_list(page_t *node) {
	return node->next;
}

static page_t *all_pages_array;
page_list_t free_pages;

/* set up pages to get basic memory allocations working */
void mem_init(atag_t *atags) {
	uint32_t mem_size, page_array_len, kernel_pages, i;

	/* get total number of pages */
	mem_size = get_mem_size(atags);
	num_pages = mem_size / PAGE_SIZE;

	/* allocate space for the metadata of all pages */
	page_array_len = sizeof(page_t) * num_pages;
	all_pages_array = (page_t *)&__end;		// pages start after sections defined in linker.ld
	bzero(all_pages_array, page_array_len);
	free_pages.head = nullptr;
	free_pages.size = 0;

	/* mark pages which are part of the kernel with the appropriate flags */
	kernel_pages = ((uint32_t)&__end) / PAGE_SIZE;
	for (i = 0; i < kernel_pages; i++) {
		all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;
		all_pages_array[i].flags.allocated = 1;
		all_pages_array[i].flags.kernel_page = 1;
	}

	/* mark all other pages, which are free */
	for (; i < num_pages; i++) {
		all_pages_array[i].flags.allocated = 0;
		append_page_list(&free_pages, &all_pages_array[i]);
	}
}

void *alloc_page(void) {
	page_t *page;
	void *page_mem;

	if (size_page_list(&free_pages) == 0)
		return 0;

	page = pop_page_list(&free_pages);
	page->flags.kernel_page = 1;
	page->flags.allocated = 1;

	page_mem = (void *)((page - all_pages_array) * PAGE_SIZE);

	bzero(page_mem, PAGE_SIZE);
	check_page(page_mem);

	return page_mem;
}

void free_page(void *ptr) {
	page_t *page;
	page = all_pages_array + ((uint32_t) ptr / PAGE_SIZE);
	page->flags.allocated = 0;
	append_page_list(&free_pages, page);
}

void check_page(void *p) {
	uint32_t addr = (uint32_t)p;
	puts("page @ ");
	puthex(addr);
	puts("\n");

	if (!p)
		puts("	FAIL: NULL\n");
	if (addr & (PAGE_SIZE - 1))
		puts("	FAIL: unaligned\n");
	if (addr < (uint32_t)&__end)
		puts("	FAIL: overlaps kernel\n");
}

#endif
