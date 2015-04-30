#include <sys/defs.h>

volatile uint64_t virt_ptr; //record the virtual memory used address

uint64_t* virt_page_allocator(int page_num, uint64_t flags);

void virt_page_freer(void* virt_addr);

void* kmmap(uint64_t start_addr, uint64_t bytes, uint64_t flags);

void *kmalloc(uint64_t size);