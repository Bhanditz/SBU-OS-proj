#include <sys/common.h>
#include <sys/sbunix.h>


void pinfo(char* info_msg) {
	printf("[INFO]: %s\n", info_msg);
}

void perror(char* err_msg) {
	printf("[ERROR]: %s\n", err_msg);
	while(1);
}

void *memset(void *addr, uint8_t value, uint64_t size) {
    uint8_t* tmp = addr;
    while(size > 0) {
        *tmp = value;
        tmp++;
        size--;
    }
    return addr;
}

uint64_t *memset_byte(uint64_t *addr, uint64_t value, uint64_t size) {

    uint64_t *temp = addr;

    while (size > 0) {
        *temp = value; 
        temp++;
        size--;
    }
    return addr;
}

void* memcpy(void *dest, void *src, uint64_t size) {
    
    uint8_t *d_ptr = (uint8_t *)dest;
    uint8_t *s_ptr = (uint8_t *)src;

    while(size) {
        *d_ptr = *s_ptr; 
        d_ptr++;
        s_ptr++;
        size--;
    }

    return dest;
}



