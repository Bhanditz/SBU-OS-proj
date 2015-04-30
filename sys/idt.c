#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>

#define MIN_IDT 256

extern void pic_remap(int, int);

struct idt_entry_descriptor idt_entry[MIN_IDT];

struct idt_ptr_t idt_ptr = {
	sizeof(struct idt_entry_descriptor) * 256 - 1,
	(uint64_t)idt_entry,
};

void _x86_64_asm_lidt(struct idt_ptr_t* idt_ptr);

void reload_idt() {
	_x86_64_asm_lidt(&idt_ptr);
}

void idt_entry_set(struct idt_entry_descriptor* idt_e, uint64_t offset, uint16_t selector, unsigned type, unsigned dpl) {
	idt_e->offset_low = offset & 0xFFFF;
	idt_e->segment_selector = selector; //must point to a selector in the GDT
	idt_e->ist = 0;
	idt_e->reserved_fivezero = 0;
	idt_e->type = type;
	idt_e->zero = 0;
	idt_e->dpl = dpl;
	idt_e->p = 1;
	idt_e->offset_mid = (offset>>16) & 0xFFFF;
	idt_e->offset_high = (offset>>32) & 0xFFFFFFFF;
	idt_e->reserved = 0;
}

void setup_idt() {
	//special handler
	idt_entry_set(&idt_entry[32], (uint64_t)&isr_timer, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[33], (uint64_t)&isr_keyboard, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[0], (uint64_t)&isr_dividedbyzero, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[10], (uint64_t)&isr_tssfault, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[13], (uint64_t)&isr_gpfault, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[14], (uint64_t)&isr_pagefault, 0x08, 0xE, 0);

	//the idt entry debugging
	idt_entry_set(&idt_entry[1], (uint64_t)&isr1, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[2], (uint64_t)&isr2, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[3], (uint64_t)&isr3, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[4], (uint64_t)&isr4, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[5], (uint64_t)&isr5, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[6], (uint64_t)&isr6, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[7], (uint64_t)&isr7, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[8], (uint64_t)&isr8, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[9], (uint64_t)&isr9, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[11], (uint64_t)&isr11, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[12], (uint64_t)&isr12, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[15], (uint64_t)&isr15, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[16], (uint64_t)&isr16, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[17], (uint64_t)&isr17, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[18], (uint64_t)&isr18, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[19], (uint64_t)&isr19, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[20], (uint64_t)&isr20, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[21], (uint64_t)&isr21, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[22], (uint64_t)&isr22, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[23], (uint64_t)&isr23, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[24], (uint64_t)&isr24, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[25], (uint64_t)&isr25, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[26], (uint64_t)&isr26, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[27], (uint64_t)&isr27, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[28], (uint64_t)&isr28, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[29], (uint64_t)&isr29, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[30], (uint64_t)&isr30, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[31], (uint64_t)&isr31, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[34], (uint64_t)&isr34, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[35], (uint64_t)&isr35, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[36], (uint64_t)&isr36, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[37], (uint64_t)&isr37, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[38], (uint64_t)&isr38, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[39], (uint64_t)&isr39, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[40], (uint64_t)&isr40, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[41], (uint64_t)&isr41, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[42], (uint64_t)&isr42, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[43], (uint64_t)&isr43, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[44], (uint64_t)&isr44, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[45], (uint64_t)&isr45, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[46], (uint64_t)&isr46, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[47], (uint64_t)&isr47, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[48], (uint64_t)&isr48, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[49], (uint64_t)&isr49, 0x08, 0xE, 0);
	idt_entry_set(&idt_entry[50], (uint64_t)&isr50, 0x08, 0xE, 0);

	//remap the pic
	disable_ints();
	pic_remap(0x20, 0x28);
	irq_clear_mask(0);
	enable_ints();

	//initiate the pit int
	pit_init();
}





