#include <sys/irq.h>
#include <sys/sbunix.h>

void pit_init() {
	uint16_t frequency = 100;
	uint16_t divisor = 1193180/frequency;

	// Send the command byte.
	outb(0x43, 0x30 | 0x06); //channel 0, lobyte/hibyte, mode 2:110b

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t lobyte = (uint8_t)(divisor & 0xFF);
	uint8_t hibyte = (uint8_t)(divisor >> 8);

	// Send the frequency divisor.
	outb(0x40, lobyte);
	outb(0x40, hibyte);
}













