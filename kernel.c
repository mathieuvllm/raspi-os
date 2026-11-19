#include <stddef.h>
#include <stdint.h>

// just because I always forget a 0...
#define ZERO_HEX 0x00000000

static inline void mmio_write(uint32_t reg, uint32_t data) {
	*(volatile uint32_t*)reg = data;
}

static inline uint32_t mmio_read(uint32 reg) {
	return *(volatile uint32_t*)reg;
}

static inline void delay(uint32_t count) {
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
	: "=r"(count): [count]"0"(count) : "cc");
}

 /* register addresses for GPIO and UART0 */
enum {
	GPIO_BASE		= 0x3F200000,
	GPPUD			= (GPIO_BASE + 0x94),
	GPPUDCLK0		= (GPIO_BASE + 0x98),

	UART0_BASE		= 0x3F201000,
	UART0_DR		= (UART0_BASE + 0x00),
	UART0_RSRECR	= (UART0_BASE + 0x04),
	UART0_FR		= (UART0_BASE + 0x18),
	UART0_ILPR		= (UART0_BASE + 0x20),
	UART0_IBRD		= (UART0_BASE + 0x24),
	UART0_FBRD		= (UART0_BASE + 0x28),
	UART0_LCRH		= (UART0_BASE + 0x2C),
	UART0_CR		= (UART0_BASE + 0x30),
	UART0_IFLS		= (UART0_BASE + 0x34),
	UART0_IMSC		= (UART0_BASE + 0x38),
	UART0_RIS		= (UART0_BASE + 0x3C),
	UART0_MIS		= (UART0_BASE + 0x40),
	UART0_ICR		= (UART0_BASE + 0x44),
	UART0_DMACR		= (UART0_BASE + 0x48),
	UART0_ITCR		= (UART0_BASE + 0x80),
	UART0_ITIP		= (UART0_BASE + 0x84),
	UART0_ITOP		= (UART0_BASE + 0x88),
	UART0_TDR		= (UART0_BASE + 0x8C),
};

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
	for (size i = 0; str[i] != '\0'; i++)
		uart_putc((unsigned char)str[i]);
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
	(void) r0;
	(void) r1;
	(void) atags;

	uart_init();
	uart_puts("hello world!\n");

	while (1) {
		uart_putc(uart_getc());
		uart_putc('\n');
	}
}
