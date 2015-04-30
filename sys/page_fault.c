#include <sys/proc.h>
#include <sys/isr.h>
#include <sys/gdt.h>
#include <sys/common.h>
#include <sys/sbunix.h>
#include <sys/paging.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>

void page_fault_handler() {

	uint64_t fault_addr;
	uint64_t *pte;
	
	read_cr2(fault_addr);
	printf("Fault Addr:%p\n", fault_addr);  

    if (fault_addr >= 0xFFFFFFFF80000000) {
        perror("<KERNEL> page fault");

    } else {
    	pte = get_pte(fault_addr);
    	if(*pte & P_PRESENT) {
    		printf("present bit is set\n");
    		//present bit is set
    		if(*pte & P_READ_WRITE) {

    		} else {
    			perror("<USER> page fault");
    		}

    	} else {
			//present bit not set
			printf("present bit not set\n");
			
	
	        perror("<USER> page fault");
	        
    	}
    	perror("<USER> page fault");
    } 

}