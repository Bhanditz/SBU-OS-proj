#include <sys/sbunix.h>
#include <sys/common.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/irq.h>
#include <sys/tarfs.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>
#include <sys/paging.h>
#include <sys/proc.h>
#include <sys/tarfs.h>
#include <sys/elf.h>
#include <sys/kstring.h>

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	uint64_t phys_size = 0;
	uint64_t phys_base = 0;

	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
			phys_base = smap->base;
            phys_size = smap->length;
		}
	}
	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	printf("phs_base: %p; physbase: %p\n", phys_base, physbase);
	printf("physfree: %p, phys_size: %d\n", physfree, phys_size);
	printf("the marginal: %p\n", phys_base+phys_size);
	// kernel starts here
	//debug
	phys_init(phys_base, (uint64_t)physfree, phys_size);
	paging_init();

	printf("_binary_tarfs_start: %p\n", &_binary_tarfs_start);
    printf("_binary_tarfs_end: %p\n", &_binary_tarfs_end);

	//------------for test-------------
	// Schedule an Primal Kernel Process 
    create_primal_proc();

    // Create an init process to invoke shell
    load_proc("bin/hello", NULL);

    //schedule_flag = 0x1;

}

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);

	reload_gdt();
	setup_tss();
	reload_idt();
	setup_idt();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	printf("!!!!! start() returned !!!!!\n");
	//clear(); 		//clear the screen
	while(1);
}
