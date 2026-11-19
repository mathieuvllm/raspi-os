#include <kernel/uart.h>
#include <common/stdio.h>
#include <kernel/mem.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
	(void) r0;
	(void) r1;
	(void) atags;

	uart_init();
	mem_init((atag_t *)atags);
	char *page = alloc_page();
	page[0] = 's';
	page[1] = 'l';
	page[2] = 't';
	page[3] = '\n';
	page[4] = '\0';

	puts("hello world!\n");
	puts(page);

	free_page(page);

	while (1) {
		putc(uart_getc());
		putc('\n');
	}
}
