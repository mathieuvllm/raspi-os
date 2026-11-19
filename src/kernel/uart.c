#ifndef UART_C
#define UART_C

#include <kernel/uart.h>

void uart_init() {
	/* disable UART HW */
	mmio_write(UART0_CR, ZERO_HEX);

	/* disable pins 14 & 15 of the GPIO */
	mmio_write(GPPUD, ZERO_HEX);
	delay(150);

	mmio_write(GPPUDCLK0, (1 << 15) | (1 << 15));
	delay(150);

	mmio_write(GPPUDCLK0, ZERO_HEX);

	/* set all flags in interrupt clear register,
	 * to clear all pending interrupts */
	mmio_write(UART0_ICR, 0x7FF);

	/* set the baud rate (115200) */
	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);

	/* set up HW to hold 8 items of data in 8-bit words */
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	/* disable interrupts from the UART */
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
			(1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	/* enable UART HW and transmission */
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(unsigned char c) {
	/* wait while transmit FIFO full */
	while (mmio_read(UART0_FR) & (1 << 5)) { }
	mmio_write(UART0_DR, c);
}

unsigned char uart_getc() {
	/* wait while receive FIFO empty */
	while (mmio_read(UART0_FR) & (1 << 4)) { }
	return mmio_read(UART0_DR);
}

void uart_puts(const char *str) {
	for (size_t i = 0; str[i] != '\0'; i++)
		uart_putc((unsigned char)str[i]);
}

#endif
