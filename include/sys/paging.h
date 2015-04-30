#include <sys/defs.h>

#define ENTRY_LIMIT 				512

//---------------entry bit setting------------------
#define P_PRESENT					0x0000000000000001
#define P_READ_WRITE				0x0000000000000002
#define	P_USER_SUPERVISOR			0x0000000000000004
#define	P_PAGE_LEVEL_WORKTHROUGH	0x0000000000000008
#define P_PAGE_LEVEL_CACHE_DISABLE	0x0000000000000010
#define	P_ACCESS					0x0000000000000020
#define P_PTE_DIRTY					0x0000000000000040
#define P_PTE_PAGE_ATTRIBUTE_TABLE	0x0000000000000080
#define P_GLOBAL_PAGE				0x0000000000000100
#define P_PML4_MBZ					0X0000000000000180
#define P_PDPE_MBZ					0X0000000000000100
#define P_AVAILABLE_TO_SOFTWARE 	0X0000000000000D00

#define P_AVAILABLE 				0x3FF0000000000000
#define P_NO_EXECUTE				0X8000000000000000

#define P_FLAG_BITS					0xFFF0000000000FFF				
#define P_ADDR_BITS					0x000FFFFFFFFFF000

//------------self reference address-----------------
//11111111 11111111 | 11111111 0 | 1111111 10 | 111111 110 | 11111 1110 | 0000 00000000						
#define PML4_SELF_REFERENCE 		0xFFFFFF7FBFDFE000
//11111111 11111111 | 11111111 0 | 1111111 10 | 111111 110 | 00000 0000 | 0000 00000000
#define PDP_SELF_REFERENCE 			0xFFFFFF7FBFC00000
//11111111 11111111 | 11111111 0 | 1111111 10 | 000000 000 | 00000 0000 | 0000 00000000
#define PD_SELF_REFERENCE  			0xFFFFFF7F80000000
//11111111 11111111 | 11111111 0 | 0000000 00 | 000000 000 | 00000 0000 | 0000 00000000
#define PT_SELF_REFERENCE  			0xFFFFFF0000000000

//-----------------self reference entry---------------
#define PML4_SR_ENTRY				0x0000FF8000000000
#define PDP_SR_ENTRY				0X0000FFFFC0000000
#define PD_SR_ENTRY 				0x0000FFFFFFE00000
#define PT_SR_ENTRY					0X0000FFFFFFFFF000

//------------------entry offset----------------------
#define PML4E_OFFSET				0x0000FF8000000000
#define PDPE_OFFSET					0X0000007FC0000000
#define PDE_OFFSET					0x000000003FE00000
#define PTE_OFFSET 					0X00000000001FF000

#define read_cr2(cr2) __asm__ __volatile__ ("movq %%cr2, %0;" : "=r"(cr2));
#define read_cr3(cr3) __asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(cr3));

uint64_t  pml4_phys_kernel_t;
uint64_t  kernel_entry;       //pml4_virt_kernel_t[511]
uint64_t* pml4_virt_kernel_t;

//----------------entry allocate method---------------
extern void load_cr3(uint64_t pml4_kernel_t);

uint64_t kaddr_v_to_p(uint64_t virt_addr);

uint64_t kaddr_p_to_v(uint64_t phys_addr);

uint64_t pml4_table_allocator();

uint64_t* pdp_table_allocator(uint64_t* pml4_table, int pml4e_offset);

uint64_t* pd_table_allocator(uint64_t* pdp_table, int pdpe_offset);

uint64_t* pt_table_allocator(uint64_t* pd_table, int pde_offset);

uint64_t* get_pml4e(uint64_t virt_addr);

uint64_t* get_pdpe(uint64_t virt_addr);

uint64_t* get_pde(uint64_t virt_addr);

uint64_t* get_pte(uint64_t virt_addr);

void pt_struct_freer(uint64_t pml4_t);

void kmmap_init();

void map_v_to_p(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);

void paging_init();

//--------------debug---------------------
uint64_t lookup(uint64_t virt_addr);

void lookup_map(uint64_t virt_addr);



