#include <sys/virt_mem.h>
#include <sys/paging.h>
#include <sys/phys_mem.h>
#include <sys/common.h>
#include <sys/sbunix.h>


uint64_t* virt_page_allocator(int page_num, uint64_t flags) {

	int index;
	uint64_t virt_addr = virt_ptr;
	for(index=0; index<page_num; index++) {
		map_v_to_p(virt_ptr, frame_allocator(), flags);
		//printf("[virt_page_allocator]: virt: %p, phys: %p\n", virt_ptr, phys_addr);
		virt_ptr += 0x1000;
	}

	return (uint64_t *)virt_addr;
}

void virt_page_freer(void* virt_addr) {
	
	uint64_t *pte;
	uint64_t phys_addr;

	pte = get_pte((uint64_t)virt_addr);
	phys_addr = *pte>>12<<12;
	frame_freer(phys_addr, 1);
	*pte = 0x0;
}

void* kmmap(uint64_t start_addr, uint64_t size, uint64_t flags) {

    uint64_t page_num;
    uint64_t cur_ptr = virt_ptr;

    virt_ptr = start_addr >> 12 << 12;
    page_num = (size >> 12) + 1;
    // allocate virt pages
    virt_page_allocator(page_num, flags);
    // restore the prev top virtual addr
    virt_ptr = cur_ptr;

    return (void*)start_addr;
}

volatile uint64_t unused_mem;
volatile uint64_t used_ptr;

void *kmalloc(uint64_t size) {
	void 			*k_addr;
	uint64_t 		page_num;

	size = size >> 4 << 4;
    size += 0X10;
	if(size > unused_mem) {
		//some bugs will happen if allocating across two pages
		page_num = size / (0x1000) + 1;
		k_addr = virt_page_allocator(page_num, P_PRESENT | P_READ_WRITE);
		if(page_num > 1) {
			unused_mem = 0;
		} else {
			unused_mem = page_num*0x1000 - size;
			used_ptr = (uint64_t)k_addr + size;
		}
		
		//printf("unused_mem: %p, used_ptr: %p\n", unused_mem, used_ptr);
		//printf("page_num: %d\n", page_num);
	} else {
		k_addr = (void *)used_ptr;
		used_ptr += size;
		unused_mem -= size;
	}

	return k_addr;
}
