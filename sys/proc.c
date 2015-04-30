#include <sys/proc.h>
#include <sys/isr.h>
#include <sys/gdt.h>
#include <sys/common.h>
#include <sys/sbunix.h>
#include <sys/paging.h>
#include <sys/kstring.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>

//----------------------- struct managerment ---------------------

task_struct* task_struct_allocator(uint64_t mode) {

	task_struct 	*ts = NULL;
	mm_struct 		*mms = NULL; 

	if(!task_struct_avail_list) {
		ts = (task_struct *)kmalloc(sizeof(task_struct));
		mms = (mm_struct *)kmalloc(sizeof(mm_struct));
		memset((void *)ts, 0, sizeof(task_struct));
		memset((void *)mms, 0, sizeof(mm_struct));
		mms->pgd_t = pml4_table_allocator();
		ts->mm_struct = mms; 
	} else {
		ts = task_struct_avail_list;
		task_struct_avail_list = task_struct_avail_list->next;
	}
	ts->pid = pid_mark;
	ts->mode = USER;
	ts->state = TASK_RUNNING;

	return ts;
}

vm_area_struct* vm_area_struct_allocator(uint64_t start_addr, uint64_t end_addr, 
	uint64_t permission, uint64_t type, uint64_t fd) {

	vm_area_struct *vma = NULL;

	if(!vma_struct_avail_list) {
		vma = (vm_area_struct *)kmalloc(sizeof(vm_area_struct));
	} else {
		vma = vma_struct_avail_list;
		vma_struct_avail_list = vma_struct_avail_list->vma_next;
	}
	vma->vma_next = NULL;
	vma->vma_start = start_addr;
	vma->vma_end = end_addr;
	vma->vma_perm = permission;
	vma->vma_type = type;
	vma->vma_fd = fd;

	return vma;
}

//empty task_struct, mm_struct and the containing list of vma struct
void proc_freer(task_struct *task_struct) {

	mm_struct *mms = task_struct->mm_struct;
	vm_area_struct *vma = mms->mmap;

	//free the task_struct
	task_struct->state = EXIT_DEAD;
	task_struct->pid = 0x0;
	task_struct->ppid = 0x0;
	task_struct->mode = 0x0;
	task_struct->rip = 0x0;
	task_struct->rsp = 0x0;
	task_struct->sleep_cnt = 0x0;
	task_struct->next = NULL;
	task_struct->prev = NULL;
	task_struct->parent = NULL;
	task_struct->child_list_header = NULL;
	task_struct->child_list_next = NULL;
	task_struct->num_children = 0x0;
	task_struct->wait_on_child_pid = 0x0;
	memset((void *)task_struct->k_stack, 0, 512);
	memset((void *)task_struct->proc_name, 0, 64);
	memset((void *)task_struct->fd, 0, sizeof(uint64_t *) * 10);

	/* some bugs happen here
	task_struct *ptr = task_struct_avail_list;
	while(ptr->next) {
		ptr = ptr->next;
	}
    ptr->next = task_struct;
    */

	//free the mm_struct
	mms->count = 0x0;
	pt_struct_freer(mms->pgd_t);
	mms->mmap = NULL;
	mms->start_code = 0x0;
	mms->end_code = 0x0;
	mms->start_data = 0x0;
	mms->end_data = 0x0;
	mms->start_brk = 0x0;
	mms->brk = 0x0;
	mms->start_stack = 0x0;
	mms->start_mmap = 0x0;
	mms->arg_start = 0x0;
	mms->arg_end = 0x0;
	mms->env_start = 0x0;
	mms->env_end = 0x0;
	mms->rss = 0x0;
	mms->total_vm = 0x0;
	mms->locked_vm = 0x0;

	//free the list of vma structs
	vm_area_struct *vma_header = vma;
	vm_area_struct *vma_next = NULL;
	while(1) {
		if(vma == NULL) break;
		vma_next = vma->vma_next;
        memset((void *)vma, 0, sizeof(vm_area_struct));
        vma->vma_next = vma_next;
        vma = vma_next;
	}
	vma_next = vma_header;
	while(vma_next->vma_next) {
		vma_next = vma_next->vma_next;
	}
	vma_next->vma_next = vma_struct_avail_list;
	vma_struct_avail_list = vma_header;
}	

void add_vma_to_mms(vm_area_struct *vma, mm_struct* mms) {

	vm_area_struct *ptr;

	if(!mms->mmap) {
		mms->mmap = vma;
	} else {
		ptr = mms->mmap;
		while(ptr->vma_next) {
			ptr = ptr->vma_next;
		}
		ptr->vma_next = vma;
	}
}

task_struct* copy_proc(task_struct* parent_proc) {

	return NULL;
}


//----------------------- process managerment ---------------------

void primal_process() {
    while(1);
}

void create_primal_proc() {
    primal_proc = task_struct_allocator(KERNEL);
    primal_proc->state = TASK_RUNNING;
    kstrcpy(primal_proc->proc_name, "primal process");
    proc_init(primal_proc, (uint64_t)&primal_process, (uint64_t)&primal_proc->k_stack[511]);
}

void add_proc(task_struct *proc) {

	task_struct *ptr = running_proc_list;
	switch(proc->state) {
		case TASK_INTERRUPTIBLE:    
		case TASK_UNINTERRUPTIBLE:
		case __TASK_STOPPED:
		case __TASK_TRACED:
		case EXIT_ZOMBIE:
		case TASK_WAKEKILL:             
		case TASK_WAKING:
			return;
		//when EXIT, add proc to task_struct_avail_list
		case EXIT_DEAD:
			proc->state = __TASK_STOPPED;
			proc->next = task_struct_avail_list;
			task_struct_avail_list = proc;
			return;
		//when RUNNING, add proc to running_proc_list
		case TASK_RUNNING:
			if(ptr) {
				while(ptr->next) {
					ptr = ptr->next;
				}
				ptr->next = proc;
			} else {
				running_proc_list = proc;
			}
			proc->next = NULL;
			return;
	}
}

//get the following running process
task_struct* get_next_proc() {

	task_struct *prev_proc = NULL;
	task_struct *next_proc = running_proc_list;

	uint64_t i = 0;
	task_struct *ts = running_proc_list;
	while(ts) {
		printf("process[%d] name: %s\n", i, ts->proc_name);
		//printf("process[%s]: rsp--%p \n", ts->proc_name, ts->rsp);
		ts = ts->next;
		i++;
	}

	//the break condition: next_proc is null/state = TASK_RUNNING
	while(next_proc && next_proc->state != TASK_RUNNING) {
		prev_proc = next_proc;
		next_proc = next_proc->next;
	}
	//there must be a proc in TASK_RUNNING, because shell proc always in list
	if(prev_proc && next_proc) {
		prev_proc->next = next_proc->next;
	}
	if(!next_proc) {
		next_proc = primal_proc;
	}
	cur_proc = next_proc;
	return next_proc;
}

void sleep_cnt_update() {

	task_struct *ptr = running_proc_list;
	while(ptr) {
		if(ptr->state == TASK_UNINTERRUPTIBLE) {
			if(ptr->sleep_cnt) {
				ptr->sleep_cnt--;
			} else {
				ptr->state = TASK_RUNNING;
			}
		}
		ptr = ptr->next;
	}
}

void context_switch(task_struct *proc) {

	load_cr3(proc->mm_struct->pgd_t);
	in_rsp(proc->rsp);
	if (proc->mode == USER) {
    	tss.rsp0 = (uint64_t)&proc->k_stack[511]; //the top addr of the stack
    	switch_to_ring3();
    }
}

//initialize the process stack and registers, then add it to proc_list
void proc_init(task_struct* proc, uint64_t entry, uint64_t stack_top) {
	// Set up kernel stack of ss, rsp, rflags, cs, rip
    if (proc->mode == USER) {
        //user process
        proc->k_stack[511] = 0x23; 
        proc->k_stack[508] = 0x1B;  
    } else if(proc->mode == KERNEL) {
        //kernel process <syscall>
        proc->k_stack[511] = 0x10; 
        proc->k_stack[508] = 0x08;
    }
    proc->k_stack[510] = stack_top;
    proc->k_stack[509] = 0x200202;
    proc->rip = proc->k_stack[507] = entry;

    /*	Leave 15 spaces for pop up all the register from 506 to 492
		Then set return address to POPA in isr_timer() and set rsp to 490
	*/
    proc->k_stack[491] = (uint64_t)&isr_timer + 0x20;
    proc->rsp = (uint64_t)&proc->k_stack[490];

    add_proc(proc);
}


















