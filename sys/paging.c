#include <sys/paging.h>
#include <sys/mem.h>
#include <sys/phys_mem.h>
#include <sys/common.h>
#include <sys/sbunix.h>

uint64_t  pml4_phys_kernel_t;
uint64_t* pml4_virt_kernel_t;

//allocate a pml4 table memory
uint64_t* pml4_table_allocator() {
	uint64_t pml4_t_phys_addr;
	uint64_t *pml4_t_virt_addr;

	pml4_t_phys_addr = frame_allocator();
	pml4_t_virt_addr = (uint64_t *)kaddr_p_to_v(pml4_t_phys_addr);

	/*[510] is the self reference entery, [511] is use to map to 
	  kernel space in every process due to it all locates in 
	  0XFFFFFFFF80000000, the last entry of pml4
	*/
	pml4_t_virt_addr[510] = pml4_t_phys_addr | P_PRESENT | P_READ_WRITE;

	return pml4_t_virt_addr;
}

//allocate a pdp table memory and add it into pml4 table entry
uint64_t* pdp_table_allocator(uint64_t* pml4_table, uint64_t pml4e_offset) {
	uint64_t pdp_t_phys_addr;
	uint64_t *pdp_t_virt_addr;

	pdp_t_phys_addr = frame_allocator();
	*(pml4_table + pml4e_offset) = pdp_t_phys_addr | P_PRESENT | P_READ_WRITE;
	pdp_t_virt_addr = (uint64_t *)kaddr_p_to_v(pdp_t_phys_addr);

	return pdp_t_virt_addr;
}

uint64_t* pd_table_allocator(uint64_t* pdp_table, uint64_t pdpe_offset) {
	uint64_t pd_t_phys_addr;
	uint64_t *pd_t_virt_addr;

	pd_t_phys_addr = frame_allocator();
	*(pdp_table + pdpe_offset) = pd_t_phys_addr | P_PRESENT | P_READ_WRITE;
	pd_t_virt_addr = (uint64_t *)kaddr_p_to_v(pd_t_phys_addr);

	return pd_t_virt_addr;
}

uint64_t* pt_table_allocator(uint64_t* pd_table, uint64_t pde_offset) {
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
    pd_table = pdp_table_allocator(pdp_table, pdpe_offset);

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
void map_v_to_p(uint64_t virt_addr, uint64_t phys_addr, uint64_t page_num) {

}


void paging_init() {

	pml4_phys_kernel_t = frame_allocator();
	pml4_virt_kernel_t = (uint64_t *)kaddr_p_to_v(pml4_phys_kernel_t);
	pml4_virt_kernel_t[510] = pml4_phys_kernel_t | P_PRESENT | P_READ_WRITE;

	pinfo("Initialize the kernel memory mapping...");
	kmmap_init();

	pinfo("Enabling the paging mode...");
	load_cr3(pml4_phys_kernel_t);
}


