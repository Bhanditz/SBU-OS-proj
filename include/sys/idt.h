#include <sys/defs.h>

// the struct describes an interrupt entry.
struct idt_entry_descriptor {
	uint64_t offset_low:16;
	uint64_t segment_selector:16;
	uint64_t ist:3;
	uint64_t reserved_fivezero:5;
	uint64_t type:4;
	uint64_t zero:1;
	uint64_t dpl:2;
	uint64_t p:1;
	uint64_t offset_mid:16;
	uint64_t offset_high:32;
	uint64_t reserved:32;
}__attribute__((packed));

void reload_idt();
void setup_idt();

// The struct points to an array of interrupt handlers.
// This format is fit for 'lidt'.
struct idt_ptr_t
{
	uint16_t size;
	// Point to the address of the first element in idt_entry_descriptor array.
	uint64_t addr; 
}__attribute__((packed));


