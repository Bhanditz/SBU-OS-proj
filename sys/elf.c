#include <sys/common.h>
#include <sys/sbunix.h>
#include <sys/kstring.h>
#include <sys/paging.h>
#include <sys/proc.h>
#include <sys/elf.h>
#include <sys/tarfs.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>


void store_proc_args(task_struct *proc, int arg_cnt) {

    int         index, len;
    uint64_t    cur_pgd_t;      //the current process table addr
    uint64_t    *arg_val[MAX_ARG];          //argv to mark the argument value
    uint64_t    *user_stack = (uint64_t *)proc->mm_struct->start_stack;

    // get the current process's page table structure
    read_cr3(cur_pgd_t);
    // load the aim process's page table
    load_cr3(proc->mm_struct->pgd_t);
    // Store the value of arguments 
    index = arg_cnt - 1;
    while(index >= 0) {
        len = kstrlen(args[index]);
        user_stack = (uint64_t*)((void*)user_stack - len);
        memcpy((char*)user_stack, args[index], len);
        arg_val[index] = user_stack;
        // printf("[arg value]argv[%d]: %s\n", i, argv[i]);
        index--;
    }
    // Store the pointers of arguments
    index = arg_cnt - 1;
    while(index >= 0) {
        user_stack--;
        *user_stack = (uint64_t)arg_val[index];
        // printf("[arg pointer]user_stack = *user_stack: %p=%s\n", user_stack, *user_stack);
        index--;
    }
    // Store the number of arguments 
    user_stack--;
    *user_stack = (uint64_t)arg_cnt;
    // printf("[arg count]user_stack = *user_stack: %p=%d\n", user_stack, *user_stack);
    // update the stack top addr
    proc->mm_struct->start_stack = (uint64_t)user_stack;
    load_cr3(cur_pgd_t);
}

//return the end addr of this program header
uint64_t load_phdr(Elf64_Ehdr* hdr, Elf64_Phdr* phdr, task_struct *proc, uint64_t cur_pgd_t) {
    
    uint64_t        vma_type, flags;
    uint64_t        start_vaddr = phdr->p_vaddr;
    uint64_t        mem_size = phdr->p_memsz;
    uint64_t        file_size = phdr->p_filesz;
    mm_struct       *mm_struct = proc->mm_struct; 
    vm_area_struct  *new_seg;   

    switch(phdr->p_flags) {
        case 5:     vma_type = TEXT;
        case 6:     vma_type = DATA;
        default:    vma_type = COMM;
    }
    // Allocate a new vma to map the program segment
    new_seg = vm_area_struct_allocator(start_vaddr, start_vaddr+mem_size, phdr->p_type, vma_type, 0); 
    //update the mm_struct
    mm_struct->count++;
    mm_struct->total_vm += mem_size;
    //printf("start:%p end:%p size:%p\n", start_vaddr, start_vaddr+size, size);

    // load ELF sections into new virtual space<load child cr3>
    load_cr3(mm_struct->pgd_t);
    if(vma_type == TEXT) {
        flags = P_PRESENT | P_USER_SUPERVISOR; 
    } else {
        flags = P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR;
    }
    kmmap(start_vaddr, mem_size, flags);
    //add the new_seg to the mm_struct
    add_vma_to_mms(new_seg, mm_struct);
    //printf("Vaddr = %p, ELF = %p, size = %p\n",(void*) start_vaddr, (void*) header + program_header->p_offset, size);
    memcpy((void *)start_vaddr, (void *)hdr + phdr->p_offset, file_size);
    // Set .bss section with zero, and only in case of segment containing .bss will filesize and memsize differ 
    memset((void *)start_vaddr + file_size, 0, mem_size - file_size);
    // Restore parent CR3
    load_cr3(cur_pgd_t);

    return start_vaddr+mem_size;
}

void stack_heap_vma_allocator(task_struct *proc, uint64_t cur_pgd_t, uint64_t max_addr) {

    uint64_t        start_vaddr, end_vaddr;
    vm_area_struct  *ptr;
    mm_struct       *mm_struct = proc->mm_struct;

    // allocate stack vma
    ptr = mm_struct->mmap;
    while(ptr->vma_next) {
        ptr = ptr->vma_next;
    }    
    start_vaddr = USER_STACK_TOP - USER_STACK_SIZE;
    end_vaddr = USER_STACK_TOP;
    ptr->vma_next = vm_area_struct_allocator(start_vaddr, end_vaddr, R | W, STACK, 0);
    // switch the context to set the mapping
    load_cr3(mm_struct->pgd_t);
    kmmap(end_vaddr-0x1000, 0x1000, P_PRESENT | P_READ_WRITE | P_USER_SUPERVISOR);
    load_cr3(cur_pgd_t);

    // allocate heap vma
    ptr = mm_struct->mmap;
    while(ptr->vma_next) {
        ptr = ptr->vma_next;
    }    
    //get the end addr of 4k align 
    start_vaddr = end_vaddr = (max_addr & ~0xFFF) + 0x1000;
    ptr->vma_next  = vm_area_struct_allocator(start_vaddr, end_vaddr, R | W, HEAP, 0);

    //update the mm_struct
    mm_struct->count += 2;
    mm_struct->locked_vm  = USER_STACK_SIZE;
    mm_struct->total_vm += USER_STACK_SIZE;
    mm_struct->start_brk = start_vaddr;
    mm_struct->brk = end_vaddr; 
    // kprintf("Heap Start:%p\n", mm_struct->start_brk);
    // Initialize stack top to end of stack VMA
    mm_struct->start_stack = USER_STACK_TOP - 0x8;
}

void load_args(task_struct *proc, char *file_name, char *arg_val[]) {

    int        arg_cnt = 0;

    kstrcpy(args[arg_cnt], file_name);
    arg_cnt++;
    if(arg_val) {
        while(arg_val[arg_cnt-1]) {
            kstrcpy(args[arg_cnt], arg_val[arg_cnt-1]);
            arg_cnt++;
        }
    }
    store_proc_args(proc, arg_cnt);
}


task_struct* load_elf(Elf64_Ehdr* hdr, task_struct *proc, char *file_name, char *arg_val[]) {

    uint64_t        cur_pgd_t, max_addr, index;
    uint64_t        end_vaddr = 0;
    Elf64_Phdr*     phdr = (Elf64_Phdr *)((void*)hdr + hdr->e_phoff);

    read_cr3(cur_pgd_t);
    kstrncpy(proc->proc_name, file_name, sizeof(proc->proc_name)-1);
    max_addr = 0; 
    //printf("hdr->e_phnum: %d\n", hdr->e_phnum);
    for (index=0; index<hdr->e_phnum; index++) {
        //printf("\np_type: %d, p_flags: %d\n", phdr->p_type, phdr->p_flags); 
        /* Loadable program segment */
        if (phdr->p_type == 1) {            
            end_vaddr = load_phdr(hdr, phdr, proc, cur_pgd_t);
            //printf("end_vaddr: %p\n", end_vaddr);
        }
        //update the vma bound
        if (max_addr < end_vaddr) {
            max_addr = end_vaddr;
        }
        phdr++;
    }
    //allocate the stack and heap vma
    stack_heap_vma_allocator(proc, cur_pgd_t, max_addr);
    load_args(proc, file_name, arg_val);

    return proc;
}

uint64_t check_elf(Elf64_Ehdr* hdr) {
/*
    printf("hdr->e_ident[0]: %p\n", hdr->e_ident[0]); 
    printf("hdr->e_ident[1]: %c\n", hdr->e_ident[1]); 
    printf("hdr->e_ident[2]: %c\n", hdr->e_ident[2]); 
    printf("hdr->e_ident[3]: %c\n", hdr->e_ident[3]); 
*/
    // Check the magic bits
    if (hdr->e_ident[0] == 0x7F && hdr->e_ident[1] == 'E' 
        && hdr->e_ident[2] == 'L' && hdr->e_ident[3] == 'F') {
        return 1;
    }   

    return 0;
}

task_struct* load_proc(char* file_name, char *arg_val[]) {

    Elf64_Ehdr *hdr = (Elf64_Ehdr *)retrieval(file_name);

    if(check_elf(hdr)) {
        task_struct *proc = task_struct_allocator(USER);
        load_elf(hdr, proc, file_name, arg_val);
        proc_init(proc, hdr->e_entry, proc->mm_struct->start_stack);
        return proc;
    } else {
        return NULL;
    }
}




















