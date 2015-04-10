#include <sys/defs.h>

void *memset(void *s, uint8_t ch, uint64_t size);

uint64_t *memset_byte(uint64_t *ptr, uint64_t value, uint64_t size);

void pinfo(char* info_msg);

void perror(char* err_msg);