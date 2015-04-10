#include <sys/mem.h>

/*Mappings the virtual address range from 0xFFFFFFFF80200000
  to physical address 0x200000 of the whole kernel space
*/
  
uint64_t kaddr_v_to_p(uint64_t virt_addr) {
	uint64_t phys_addr;

	phys_addr = (virt_addr >> 12 << 12) - KERNEL_SPACE_BASE;

	return phys_addr;
}

uint64_t kaddr_p_to_v(uint64_t phys_addr) {
	uint64_t virt_addr;

	virt_addr = (phys_addr >> 12 << 12) + KERNEL_SPACE_BASE;

	return virt_addr;
}