#include <tests/mem_tests.h>
#include <kernel/mem.h>
#include <common/stdio.h>
#include <stdint.h>
#include <common/debug.h>

extern uint8_t __end;

static test_ret_t page_test();

static test_ret_t check_page_test(void *p);

test_ret_t run_mem_tests() {
	puts("mem_tests\r\n");
	if (page_test() == TEST_FAIL) {
		puts("[FAIL] page_test\r\n");
		return TEST_FAIL;
	}
	puts("[PASS] page_test\r\n");

	puts("mem_tests OK\r\n");
	return TEST_OK;
}

test_ret_t page_test() {
	char *a = alloc_page();
	ASSERT(check_page_test(a));
	char *b = alloc_page();
	ASSERT(check_page_test(a));
	ASSERT(a != b);
	ASSERT(a >= b + PAGE_SIZE || b >= a + PAGE_SIZE);
	for (int i = 0; i < PAGE_SIZE; i++) a[i] = (char)i;
	for (int i = 0; i < PAGE_SIZE; i++)
		ASSERT(a[i] == (char)i);

	free_page(a);
	char *c = alloc_page();
	ASSERT(check_page_test(c));
	ASSERT(c == a);
	for (int i = 0; i < PAGE_SIZE; i++)
		ASSERT(c[i] == 0);
	free_page(b);
	free_page(c);

	/* exhaustion test */
	uint32_t n = 0;
	while (alloc_page()) n++;
	ASSERT(n == (mem_size - kernel_end) / PAGE_SIZE);

	return TEST_OK;
}

test_ret_t check_page_test(void *p) {
	uint32_t addr = (uint32_t)p;
	ASSERT(p);
	ASSERT(!(addr & (PAGE_SIZE - 1)));
	ASSERT(addr >= (uint32_t)&__end);
	return TEST_OK;
}
