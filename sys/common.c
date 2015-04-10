#include <sys/common.h>
#include <sys/sbunix.h>

void *memset(void *s, uint8_t value, uint64_t size) {
    uint8_t* tmp = s;
    while(size > 0) {
        *tmp = value;
        tmp++;
        size--;
    }
    return s;
}

uint64_t *memset_byte(uint64_t *s, uint64_t value, uint64_t size) {

    uint64_t *temp = s;

    while (size > 0) {
        *temp = value; 
        temp++;
        size--;
    }
    return s;
}

void pinfo(char* info_msg) {
	printf("[INFO]: %s\n", info_msg);
}

void perror(char* err_msg) {
	printf("[ERROR]: %s\n", err_msg);
	while(1);
}