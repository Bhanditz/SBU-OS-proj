#include <sys/proc.h>
#include <sys/isr.h>
#include <sys/pic.h>
#include <sys/gdt.h>
#include <sys/common.h>
#include <sys/sbunix.h>
#include <sys/paging.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>

//isr 0
void divided_by_zero_handler() {
	perror("Cannot divided by zero");
}
//isr 10
void tss_fault_handler() {
    perror("Invalid tss");
}
//isr 13
void gp_fault_handler() {
	uint64_t fault_addr;
	read_cr2(fault_addr);
	printf("cr2: %p\n", fault_addr);
	read_cr3(fault_addr);
	printf("cr3: %p\n", fault_addr);
    perror("General protection fault");
}

//----------General----------------
void isr_1() {
	printf("INT 1 haapen!\n");
	pic_sendEOI(1);
}
void isr_2() {
	printf("INT 2 haapen!\n");
	pic_sendEOI(2);
}
void isr_3() {
	printf("INT 3 haapen!\n");
	pic_sendEOI(3);
}
void isr_4() {
	printf("INT 4 haapen!\n");
	pic_sendEOI(4);
}
void isr_5() {
	printf("INT 5 haapen!\n");
	pic_sendEOI(5);
}
void isr_6() {
	printf("INT 6 haapen!\n");
	pic_sendEOI(6);
}
void isr_7() {
	printf("INT 7 haapen!\n");
	pic_sendEOI(7);
}
void isr_8() {
	printf("INT 8 haapen!\n");
	pic_sendEOI(8);
}
void isr_9() {
	printf("INT 9 haapen!\n");
	pic_sendEOI(9);
}
void isr_11() {
	printf("INT 11 haapen!\n");
	pic_sendEOI(11);
}
void isr_12() {
	printf("INT 12 haapen!\n");
	pic_sendEOI(12);
}
void isr_15() {
	printf("INT 15 haapen!\n");
	pic_sendEOI(15);
}
void isr_16() {
	printf("INT 16 haapen!\n");
	pic_sendEOI(16);
}
void isr_17() {
	printf("INT 17 haapen!\n");
	pic_sendEOI(17);
}
void isr_18() {
	printf("INT 18 haapen!\n");
	pic_sendEOI(18);
}
void isr_19() {
	printf("INT 19 haapen!\n");
	pic_sendEOI(19);
}
void isr_20() {
	printf("INT 20 haapen!\n");
	pic_sendEOI(20);
}
void isr_21() {
	printf("INT 21 haapen!\n");
	pic_sendEOI(21);
}
void isr_22() {
	printf("INT 22 haapen!\n");
	pic_sendEOI(22);
}
void isr_23() {
	printf("INT 23 haapen!\n");
	pic_sendEOI(23);
}
void isr_24() {
	printf("INT 24 haapen!\n");
	pic_sendEOI(24);
}
void isr_25() {
	printf("INT 25 haapen!\n");
	pic_sendEOI(25);
}
void isr_26() {
	printf("INT 26 haapen!\n");
	pic_sendEOI(26);
}
void isr_27() {
	printf("INT 27 haapen!\n");
	pic_sendEOI(27);
}
void isr_28() {
	printf("INT 28 haapen!\n");
	pic_sendEOI(28);
}
void isr_29() {
	printf("INT 29 haapen!\n");
	pic_sendEOI(29);
}
void isr_30() {
	printf("INT 30 haapen!\n");
	pic_sendEOI(30);
}
void isr_31() {
	printf("INT 31 haapen!\n");
	pic_sendEOI(31);
}
void isr_34() {
	printf("INT 34 haapen!\n");
	pic_sendEOI(34);
}
void isr_35() {
	printf("INT 35 haapen!\n");
	pic_sendEOI(35);
}
void isr_36() {
	printf("INT 36 haapen!\n");
	pic_sendEOI(36);
}
void isr_37() {
	printf("INT 37 haapen!\n");
	pic_sendEOI(37);
}
void isr_38() {
	printf("INT 38 haapen!\n");
	pic_sendEOI(38);
}
void isr_39() {
	printf("INT 39 haapen!\n");
	pic_sendEOI(39);
}
void isr_40() {
	printf("INT 40 haapen!\n");
	pic_sendEOI(40);
}
void isr_41() {
	printf("INT 41 haapen!\n");
	pic_sendEOI(41);
}
void isr_42() {
	printf("INT 42 haapen!\n");
	pic_sendEOI(42);
}
void isr_43() {
	printf("INT 43 haapen!\n");
	pic_sendEOI(43);
}
void isr_44() {
	printf("INT 44 haapen!\n");
	pic_sendEOI(44);
}
void isr_45() {
	printf("INT 45 haapen!\n");
	pic_sendEOI(45);
}
void isr_46() {
	printf("INT 46 haapen!\n");
	pic_sendEOI(46);
}
void isr_47() {
	printf("INT 47 haapen!\n");
	pic_sendEOI(47);
}
void isr_48() {
	printf("INT 48 haapen!\n");
	pic_sendEOI(48);
}
void isr_49() {
	printf("INT 49 haapen!\n");
	pic_sendEOI(49);
}
void isr_50() {
	printf("INT 50 haapen!\n");
	pic_sendEOI(50);
}

