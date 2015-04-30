#include <sys/defs.h>

#define KERNEL_SIZE 				0x400000
#define KERNEL_SPACE_BASE 			0xFFFFFFFF80000000

void phys_init(uint64_t phys_base, uint64_t phys_free, uint64_t phys_size);

uint64_t frame_allocator(); 

void frame_freer(uint64_t phys_addr, uint64_t zero);




