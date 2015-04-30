#include <sys/phys_mem.h>
#include <sys/virt_mem.h>
#include <sys/paging.h>
#include <sys/common.h>
#include <sys/sbunix.h>


volatile uint64_t 	free_frame_num;
volatile uint64_t 	max_frame_num;
volatile uint64_t 	frame_mem_size;
volatile uint64_t 	frame_mem_base;
volatile uint64_t  	frame_mem_ceil;
volatile uint64_t* 	frame_allocator_map;
volatile uint8_t*   frame_reference;



void phys_init(uint64_t phys_base, uint64_t phys_free, uint64_t phys_size) {

	frame_mem_base = phys_base + KERNEL_SIZE;
	printf("frame_mem_base: %p\n", frame_mem_base);
	frame_mem_size = phys_size - KERNEL_SIZE;
	frame_mem_ceil = frame_mem_base + frame_mem_size;
	free_frame_num = max_frame_num = frame_mem_size >> 12; //each frame 4kb, 2^12
	printf("max_frame_num: %d\n", max_frame_num);

	//pinfo("Initialize the physical frame memory...");
	//memset_byte((uint64_t *)frame_mem_base, 0, frame_mem_size);

	pinfo("Initialize the frame allocator...");
	/*boot page table has mapped the kernel space from phys 0x200000 to
	  virual 0xffffffff80200000
	*/
	frame_allocator_map = (uint64_t *)(KERNEL_SPACE_BASE + phys_free);
	memset_byte((uint64_t *)frame_allocator_map, 0, max_frame_num);

	frame_reference = (uint8_t*)frame_allocator_map + (max_frame_num>>3) + 2;
    memset((void*)frame_reference, 0, max_frame_num);
}

uint64_t frame_allocator() {

	uint32_t offset, bit_offset;
	uint64_t frame_addr = 0;

	//check free physmem
	if(free_frame_num <= 5) {
		return 0;
	}
	for(offset = 0; offset < (max_frame_num>>6); offset++) {
		for(bit_offset = 0; bit_offset < 64; bit_offset++) {
			if((frame_allocator_map[offset] & (1 << bit_offset)) == 0) {
				//mark this bit into used
				frame_allocator_map[offset] |= (1 << bit_offset);
				//calculate the offset to get the frame address
				frame_addr = frame_mem_base + (((offset << 6) + bit_offset) << 12);
				//reduce the free frame number record
				free_frame_num--;
				//increase the frame reference
				frame_reference[(frame_addr-frame_mem_base)>>12]++;
				break;
			}
		}
		if(frame_addr != 0) {
			break;
		}
	}

	return frame_addr;
}

void frame_freer(uint64_t phys_addr, uint64_t zero) {

	uint32_t frame_index, offset, bit_offset;
	uint64_t virt_addr = virt_ptr;
	uint64_t *pte;

	if(phys_addr < frame_mem_base || phys_addr >= frame_mem_ceil) {
		perror("Segmentation fault");
	}

	frame_index = (phys_addr - frame_mem_base) >> 12;
	offset = frame_index >> 6;
	bit_offset = frame_index & 0x3F; //equal to frame_index % 64

	frame_reference[frame_index]--;
	if(!frame_reference[frame_index]){
		if(zero) {
			//empty the frame from virtual address
			map_v_to_p(virt_addr, phys_addr, P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR);
			memset((void*)virt_addr, 0, 0x1000);
			pte = get_pte(virt_addr);
			*pte = 0x0;
		}
		//mark the bit into free
		frame_allocator_map[offset] &= ~(1 << bit_offset);
		//increase the free frame number record
		free_frame_num++;
	}

}






