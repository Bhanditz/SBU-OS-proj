#include <sys/defs.h>

void pinfo(char* info_msg);

void perror(char* err_msg);

void *memset(void *addr, uint8_t ch, uint64_t size);

uint64_t *memset_byte(uint64_t *addr, uint64_t value, uint64_t size);

void* memcpy(void *dest, void *src, uint64_t size);

void dump_regs();