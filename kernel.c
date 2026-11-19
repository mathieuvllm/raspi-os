#include <stddef.h>
#include <stdint.h>

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

// Register addresses for GPIO and UART
enum {
	GPIO_BASE		= 0x3F200000,
	GPPUD			= (GPIO_BASE + 0x94),
	GPPUDCLK0		= (GPIO_BASE + 0x98),

	UART_BASE		= 0x3F201000,
	UART_DR			= (UART_BASE + 0x00),
	UART_RSRECR		= (UART_BASE + 0x04),
	UART_FR			= (UART_BASE + 0x18),
	UART_ILPR		= (UART_BASE + 0x20),
	UART_IBRD		= (UART_BASE + 0x24),
	UART_FBRD		= (UART_BASE + 0x28),
	UART_LCRH		= (UART_BASE + 0x2C),
	UART_CR			= (UART_BASE + 0x30),
	UART_IFLS		= (UART_BASE + 0x34),
	UART_IMSC		= (UART_BASE + 0x38),
	UART_RIS		= (UART_BASE + 0x3C),
	UART_MIS		= (UART_BASE + 0x40),
	UART_ICR		= (UART_BASE + 0x44),
	UART_DMACR		= (UART_BASE + 0x48),
	UART_ITCR		= (UART_BASE + 0x80),
	UART_ITIP		= (UART_BASE + 0x84),
	UART_ITOP		= (UART_BASE + 0x88),
	UART_TDR		= (UART_BASE + 0x8C),
};
