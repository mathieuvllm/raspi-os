#include <kernel/uart.h>
#include <common/stdio.h>
#include <kernel/mem.h>
#include <common/debug.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
	(void) r0;
	(void) r1;
	uint32_t *w = (uint32_t *)atags;

	uart_init();
	mem_init((atag_t *)atags);

#ifdef KERNEL_TEST
	run_tests();
	qemu_exit(ok);
#endif


	puts("going into halt...\r\n");

	while (1) {
		putc(uart_getc());
		putc('\n');
	}
}
