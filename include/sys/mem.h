#include <sys/defs.h>

#define KERNEL_SIZE 		0x400000
#define KERNEL_SPACE_BASE 	0xFFFFFFFF80000000

uint64_t kaddr_v_to_p(uint64_t virt_addr);

uint64_t kaddr_p_to_v(uint64_t phys_addr);
