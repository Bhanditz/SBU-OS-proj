#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)

typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t; 


static inline void disable_ints() {
	__asm__ __volatile__("cli");
}

static inline void enable_ints() {
	__asm__ __volatile__("sti");
}

static inline void outb(uint16_t port, uint8_t val) {   
    __asm__ __volatile__( "outb %0, %1" 
    				: : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ __volatile__( "inb %1, %0" 
    				: "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void io_wait(void) {
    __asm__ __volatile__( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#endif
