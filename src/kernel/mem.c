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

typedef struct page_list {
	page_t *head;
	uint32_t size;
} page_list_t;

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

	if (res == nullptr)
		return nullptr;

	list->head = res->next;
	res->next = nullptr;
	list->size--;

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
uint32_t mem_size;
uint32_t kernel_end;

#define PAGE_ALIGN_UP(x) (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/* set up pages to get basic memory allocations working */
void mem_init(atag_t *atags) {
	uint32_t page_array_len, page_array_end, kernel_pages, i;

	mem_size = get_mem_size(atags);
	num_pages = mem_size / PAGE_SIZE;
	page_array_len = sizeof(page_t) * num_pages;
	all_pages_array = (page_t *)&__end;
	page_array_end = (uint32_t)&__end + page_array_len;
	kernel_end = page_array_end + PAGE_SIZE - 1;
	kernel_pages = PAGE_ALIGN_UP(kernel_end) / PAGE_SIZE;

	bzero(all_pages_array, page_array_len);
	free_pages.head = nullptr;
	free_pages.size = 0;

	for (i = 0; i < kernel_pages; i++) {
		all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;
		all_pages_array[i].flags.allocated = 1;
		all_pages_array[i].flags.kernel_page = 1;
	}

	/* mark all other pages, which are free */
	for (; i < num_pages; i++) {
		all_pages_array[i].flags.allocated = 0;
		push_page_list(&free_pages, &all_pages_array[i]);
	}
}

void *alloc_page(void) {
	page_t *page;
	void *page_mem;

	if (size_page_list(&free_pages) == 0)
		return nullptr;

	page = pop_page_list(&free_pages);
	if (page == nullptr) {
		debug_puts("null page\r\n");
		return nullptr;
	}
	page->flags.kernel_page = 1;
	page->flags.allocated = 1;

	page_mem = (void *)((page - all_pages_array) * PAGE_SIZE);

	bzero(page_mem, PAGE_SIZE);

	return page_mem;
}

void free_page(void *ptr) {
	uint32_t *p = (uint32_t *)ptr;
	if (*p == 0xDEADBEEF) return;
	page_t *page;
	page = all_pages_array + ((uint32_t) ptr / PAGE_SIZE);
	*p = 0xDEADBEEF;	// used to check use after free or double free
	page->flags.allocated = 0;
	push_page_list(&free_pages, page);
}

#endif
