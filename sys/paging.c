#include <sys/paging.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>
#include <sys/common.h>
#include <sys/sbunix.h>

uint64_t  pml4_phys_kernel_t;
uint64_t* pml4_virt_kernel_t;

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

//allocate a pml4 table memory
/*	this allocator is different from other table allocator,
	because this function is envoked after paging is enable,
	and the return should be the pml4 table's physical address 
	in order to put it into cr3
*/
uint64_t pml4_table_allocator() {
	uint64_t pml4_t_phys_addr;
	uint64_t *pml4_t_virt_addr;

	pml4_t_phys_addr = frame_allocator();
	pml4_t_virt_addr = (uint64_t *)virt_ptr;
	map_v_to_p((uint64_t)pml4_t_virt_addr, pml4_t_phys_addr, P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR);
	virt_ptr += 0x1000;

	/*[510] is the self reference entery, [511] is use to map to 
	  kernel space in every process due to it all locates in 
	  0XFFFFFFFF80000000, the last entry of pml4
	*/
	pml4_t_virt_addr[510] = pml4_t_phys_addr | P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR;
	//kernel at the same virtual place for all process
	pml4_t_virt_addr[511] = pml4_virt_kernel_t[511];
	//printf("[pml4_table_allocator]pml4_virt_kernel_t[511]: %p\n", &pml4_virt_kernel_t[511]);
	return pml4_t_phys_addr;
}

//allocate a pdp table memory and add it into pml4 table entry
uint64_t* pdp_table_allocator(uint64_t* pml4_table, int pml4e_offset) {
	uint64_t pdp_t_phys_addr;
	uint64_t *pdp_t_virt_addr;

	pdp_t_phys_addr = frame_allocator();
	*(pml4_table + pml4e_offset) = pdp_t_phys_addr | P_PRESENT | P_READ_WRITE;
	pdp_t_virt_addr = (uint64_t *)kaddr_p_to_v(pdp_t_phys_addr);

	return pdp_t_virt_addr;
}

uint64_t* pd_table_allocator(uint64_t* pdp_table, int pdpe_offset) {
	uint64_t pd_t_phys_addr;
	uint64_t *pd_t_virt_addr;

	pd_t_phys_addr = frame_allocator();
	*(pdp_table + pdpe_offset) = pd_t_phys_addr | P_PRESENT | P_READ_WRITE;
	pd_t_virt_addr = (uint64_t *)kaddr_p_to_v(pd_t_phys_addr);

	return pd_t_virt_addr;
}

uint64_t* pt_table_allocator(uint64_t* pd_table, int pde_offset) {
	uint64_t pt_t_phys_addr;
	uint64_t *pt_t_virt_addr;

	pt_t_phys_addr = frame_allocator();
	*(pd_table + pde_offset) = pt_t_phys_addr | P_PRESENT | P_READ_WRITE;
	pt_t_virt_addr = (uint64_t *)kaddr_p_to_v(pt_t_phys_addr);

	return pt_t_virt_addr;
}

uint64_t* get_pml4e(uint64_t virt_addr) {
    uint64_t offset;
    uint64_t entry_ref_addr;

    offset = (virt_addr & PML4_SR_ENTRY) >> 36 ;
    entry_ref_addr = offset | PML4_SELF_REFERENCE;
    
    return (uint64_t *)entry_ref_addr;
}

uint64_t* get_pdpe(uint64_t virt_addr) {
    uint64_t offset;
    uint64_t entry_ref_addr;

    offset = (virt_addr & PDP_SR_ENTRY) >> 27 ;
    entry_ref_addr = offset | PDP_SELF_REFERENCE;
    
    return (uint64_t *)entry_ref_addr;
}

uint64_t* get_pde(uint64_t virt_addr) {
    uint64_t offset;
    uint64_t entry_ref_addr;

    offset = (virt_addr & PD_SR_ENTRY) >> 18 ;
    entry_ref_addr = offset | PD_SELF_REFERENCE;
    
    return (uint64_t *)entry_ref_addr;
}

uint64_t* get_pte(uint64_t virt_addr) {
    uint64_t offset;
    uint64_t entry_ref_addr;

    offset = (virt_addr & PT_SR_ENTRY) >> 9 ;
    entry_ref_addr = offset | PT_SELF_REFERENCE;
    
    return (uint64_t *)entry_ref_addr;
}

//when a process exits, it needs to empty its related page table structures
void pt_struct_freer(uint64_t pml4_t) {
	/*	First enter the whole structure from pml4 entry 0 to 509,
		since 510 points to the phys address of this pml4, 
		and 511 points to the kernel codes
	*/
	int 		pml4e, pdpe, pde, pte, offset;
	uint64_t	*pml4e_e, *pdpe_e, *pde_e, *pte_e;

	for (pml4e=0; pml4e<510; pml4e++) {
		offset = pml4e<<3;
        pml4e_e = (uint64_t *)(PML4_SELF_REFERENCE | offset);
        if (*pml4e_e & P_PRESENT) {
            for (pdpe=0; pdpe<512; pdpe++) {
            	offset = (pml4e<<12) | (pdpe<<3);
                pdpe_e = (uint64_t *)(PDP_SELF_REFERENCE | offset);
                if (*pdpe_e & P_PRESENT) {
                    for (pde=0; pde<512; pde++) {
                    	offset = (pml4e<<21) | (pdpe<<12) | (pde<<3);
                        pde_e = (uint64_t *)(PD_SELF_REFERENCE | offset);
                        if (*pde_e & P_PRESENT) {
                            for (pte=0; pte<512; pte++) {
                            	offset = (pml4e<<30) | (pdpe<<21) | (pde<<12) | (pte<<3);
                                pte_e = (uint64_t *)(PT_SELF_REFERENCE | offset);
                                if (*pte_e & P_PRESENT) {
                                    virt_ptr += 0x1000;
                                    frame_freer(*pte_e & P_ADDR_BITS, 1);
                                    *pte_e = 0x0;
                                }
                            }
                            frame_freer(*pde_e & P_ADDR_BITS, 1);
                            *pde_e = 0x0;
                        }
                    }
                    frame_freer(*pdpe_e & P_ADDR_BITS, 1);
                    *pdpe_e = 0x0;
                }
            }
            frame_freer(*pml4e_e & P_ADDR_BITS, 1);
            *pml4e_e = 0x0;
        }
    }
    load_cr3(pml4_phys_kernel_t);
}

/*Before paging enable, map from 0xFFFFFFFF80200000 ~ 0xFFFFFFFF80506000, 
  to 0x200000 ~ 0x506000,
  and map from 0xFFFFFFFF800B8000 to 0xB8000 
*/ 
void kmmap_init() {  
	uint64_t *pdp_table, *pd_table, *pt_table;
	uint64_t index, phys_addr;
	uint64_t pml4e_offset, pdpe_offset; // pde_offset, pte_offset;
	/*
	pml4e_offset = (0xFFFFFFFF80200000 & PML4E_OFFSET) >> 39;
	pdpe_offset = (0xFFFFFFFF80200000 & PDPE_OFFSET) >> 30;
	pde_offset = (0xFFFFFFFF80200000 & PDE_OFFSET) >> 21;
	pte_offset = (0xFFFFFFFF80200000 & PTE_OFFSET) >> 12;
	printf("[0xFFFFFFFF80200000]: pml4e_offset: %d, pdpe_offset: %d, pde_offset: %d, pte_offset: %d\n", pml4e_offset, pdpe_offset, pde_offset, pte_offset);

	pml4e_offset = (0xFFFFFFFF80506000 & PML4E_OFFSET) >> 39;
	pdpe_offset = (0xFFFFFFFF80506000 & PDPE_OFFSET) >> 30;
	pde_offset = (0xFFFFFFFF80506000 & PDE_OFFSET) >> 21;
	pte_offset = (0xFFFFFFFF80506000 & PTE_OFFSET) >> 12;
	printf("[0xFFFFFFFF80506000]: pml4e_offset: %d, pdpe_offset: %d, pde_offset: %d, pte_offset: %d\n", pml4e_offset, pdpe_offset, pde_offset, pte_offset);

	pml4e_offset = (0xFFFFFFFF800B8000 & PML4E_OFFSET) >> 39;
	pdpe_offset = (0xFFFFFFFF800B8000 & PDPE_OFFSET) >> 30;
	pde_offset = (0xFFFFFFFF800B8000 & PDE_OFFSET) >> 21;
	pte_offset = (0xFFFFFFFF800B8000 & PTE_OFFSET) >> 12;
	printf("[0xFFFFFFFF800B8000]: pml4e_offset: %d, pdpe_offset: %d, pde_offset: %d, pte_offset: %d\n", pml4e_offset, pdpe_offset, pde_offset, pte_offset);
*/
	//same pml4 entry and same pdp entry
	pml4e_offset = (0xFFFFFFFF80000000 & PML4E_OFFSET) >> 39;
	pdpe_offset = (0xFFFFFFFF80000000 & PDPE_OFFSET) >> 30;
	pdp_table = pdp_table_allocator(pml4_virt_kernel_t, pml4e_offset);
    pd_table = pd_table_allocator(pdp_table, pdpe_offset);

    //map from 0xFFFFFFFF800B8000 to 0xB8000, pde offset = 0, pte offset = 184
	pt_table = pt_table_allocator(pd_table, 0);
	*(pt_table + 184) = 0xB8000 | P_PRESENT | P_READ_WRITE;

	//map from 0xFFFFFFFF80200000 ~ 0xFFFFFFFF80506000 to 0x200000 ~ 0x506000
    phys_addr = 0x200000;  
	pt_table = pt_table_allocator(pd_table, 1);
    for(index = 0; index < 512; index++) { 
        *(pt_table + index) = phys_addr | P_PRESENT | P_READ_WRITE;
        phys_addr += 0x1000;
    }
    pt_table = pt_table_allocator(pd_table, 2);
    for(index = 0; index < 262; index++) { 
        *(pt_table + index) = phys_addr | P_PRESENT | P_READ_WRITE;
        phys_addr += 0x1000;
    }
}

//use for debugging
uint64_t lookup(uint64_t virt_addr) {

	uint64_t tmp;
	uint64_t *pdp_table, *pd_table, *pt_table;
	uint64_t pml4e_offset, pdpe_offset, pde_offset, pte_offset;


	pml4e_offset = (virt_addr & PML4E_OFFSET) >> 39;
	pdpe_offset = (virt_addr & PDPE_OFFSET) >> 30;
	pde_offset = (virt_addr & PDE_OFFSET) >> 21;
	pte_offset = (virt_addr & PTE_OFFSET) >> 12;
	//printf("pml4e_offset: %d, pdpe_offset: %d, pde_offset:%d\n", pml4e_offset, pdpe_offset, pde_offset);

	tmp = *(pml4_virt_kernel_t + pml4e_offset);
	pdp_table = (uint64_t *)(tmp>>12<<12);
	tmp = *(pdp_table + pdpe_offset);
	pd_table = (uint64_t *)(tmp>>12<<12);
	tmp = *(pd_table + pde_offset);
	pt_table = (uint64_t *)(tmp>>12<<12);
	tmp = *(pt_table + pte_offset);
	printf("%p\n", tmp>>12<<12);

	return tmp>>12<<12;
}

//after paging enable
void map_v_to_p(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags) {

	uint64_t *pml4e, *pdpe, *pde, *pte;
	uint32_t flag = 0;

	pml4e = get_pml4e(virt_addr);
    pdpe = get_pdpe(virt_addr); 
    pde = get_pde(virt_addr);   
    pte = get_pte(virt_addr);

    if(!*pml4e & P_PRESENT) {
    	flag = 4;
    } else if(!*pdpe & P_PRESENT) {
    	flag = 3;
    } else if(!*pde & P_PRESENT) {
    	flag = 2;
    } else if(!*pte & P_PRESENT) {
    	flag = 1;
    } else if(*pte & P_PRESENT) {
    	frame_freer(phys_addr, 0);
    }

	if(flag == 4) {
    	*pml4e = frame_allocator() | P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR;
    	flag--;
    }
    if(flag == 3) {
    	*pdpe = frame_allocator() | P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR;
    	flag--;
    }
	if(flag == 2) {
    	*pde = frame_allocator() | P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR;
    	flag--;
    }
    if(flag == 1) {
    	*pte = phys_addr | flags;
    }
}


//use for debug
void lookup_map(uint64_t virt_addr) {

	uint64_t *pte;
	uint64_t content;

	pte = get_pte(virt_addr);
	content = *pte;
	printf("%p\n", content>>12<<12);
}	


void paging_init() {

	pml4_phys_kernel_t = frame_allocator();
	pml4_virt_kernel_t = (uint64_t *)kaddr_p_to_v(pml4_phys_kernel_t);
	pml4_virt_kernel_t[510] = pml4_phys_kernel_t | P_PRESENT | P_READ_WRITE;
	//printf("[paging_init]pml4_virt_kernel_t[511]: %p\n", pml4_virt_kernel_t[511]);
	pinfo("Initialize the kernel memory mapping...");
	kmmap_init();
	//printf("[paging_init]kernel_entry: %p\n", kernel_entry);

	pinfo("Enabling the paging mode...");
	load_cr3(pml4_phys_kernel_t);

	//set up the virtual memory base
	virt_ptr = 0xffffffff80510000;

}


