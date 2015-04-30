#include <sys/defs.h>

//--------------VMA STRUCT---------------
/*-----TYPE-----*/
#define TEXT						1
#define DATA 						2
#define HEAP						4
#define STACK  						8
#define ANON 						16
#define FILETYPE					32
#define COMM						0

/*-----PERMISSION-----*/
#define NONE 						0x0000000000000000
// last 8 bit: 00000100
#define R 							0x0000000000000004
// last 8 bit: 00000010
#define W  							0x0000000000000002
// last 8 bit: 00000001
#define X 							0x0000000000000001

//--------------TASK STRUCT--------------
/*-----STATE-----*/
#define TASK_RUNNING                0
#define TASK_INTERRUPTIBLE        	1
#define TASK_UNINTERRUPTIBLE      	2 		//sleep
#define __TASK_STOPPED          	4		
#define __TASK_TRACED               8		
/* in tsk->exit_state */
#define EXIT_ZOMBIE                	16
#define EXIT_DEAD                	32
/* in tsk->state again */
#define TASK_DEAD                	64 		//devided into zombie and dead
#define TASK_WAKEKILL             	128
#define TASK_WAKING                	256 

/*-----MODE-----*/
#define KERNEL 						0
#define USER 						1

#define USER_STACK_TOP              0x000000F000000000
#define USER_STACK_SIZE             0x10000  //16 pages

typedef struct vm_area_struct 	vm_area_struct;
typedef struct mm_struct 		mm_struct;
typedef struct task_struct 		task_struct;

//---------global variables----------
volatile uint64_t 			pid_mark;
task_struct 				*task_struct_avail_list;
vm_area_struct 				*vma_struct_avail_list;

uint64_t					schedule_flag; //control the schedule on/off
task_struct 				*primal_proc;
task_struct 				*cur_proc;
task_struct 				*running_proc_list; //always runs shell in it


struct vm_area_struct {
	struct mm_struct 		*mm_struct;			// mm_struct this vma belongs to
    uint64_t 				vma_start;         	// the start address of vma 
    uint64_t 				vma_end;           	// one more byte to the end address of vma
    uint64_t	 			vma_perm;         	// flags permissions: r, w, x
    uint64_t 				vma_offset;    		
    uint64_t 				vma_fd;
    uint64_t 				vma_type;           // type of segment its reffering to
    struct vm_area_struct  	*vma_next;			// point to the next vm_area_struct
};

struct mm_struct {
	uint64_t 				count;				// record num of vma
	uint64_t				pgd_t;				// point to page table of this process
	struct vm_area_struct  	*mmap;				// point to the first vma in mm_struct
	uint64_t				start_code, end_code, start_data, end_data;
	uint64_t			 	start_brk, brk, start_stack, start_mmap;
    uint64_t 				arg_start, arg_end, env_start, env_end;
	uint64_t				rss, total_vm, locked_vm;
};

struct task_struct {
    uint32_t 				pid;
    uint32_t	 			ppid;
   	uint64_t 				mode;				// 0 kernel, 1 user 
    uint64_t 				k_stack[512];
    uint64_t 				rip;
    uint64_t 				rsp;
    volatile uint64_t       state;              // -1 unrunnable, 0 runnable, >0 stopped
    task_struct 			*next_ts;
    struct mm_struct		*mm_struct; 
    char 					proc_name[64]; 		// Name of process            
    uint64_t 				sleep_cnt;         	// Centiseconds for sleep
    struct task_struct		*next;              // The next process to run in the process list
    struct task_struct		*prev;             	// The process that ran previous
    struct task_struct		*parent;           	// Keep track of parent process on fork
    task_struct				*child_list_header; // the header of child linkedlist
    task_struct				*child_list_next; 	// points to the next child task struct 
    uint64_t				*fd[10];			// array of file descriptor pointers
    uint64_t 				num_children;      	// Number of children
    uint64_t 				wait_on_child_pid;	// pid of child last exited
};

task_struct* task_struct_allocator(uint64_t mode);
vm_area_struct* vm_area_struct_allocator(uint64_t start_addr, uint64_t end_addr, 
	uint64_t permission, uint64_t type, uint64_t fd);
void proc_freer(task_struct *task_struct);
void add_vma_to_mms(vm_area_struct *vma, mm_struct* mms);
void primal_process();
void create_primal_proc();
void add_proc(task_struct *proc);
task_struct* get_next_proc();
void sleep_cnt_update();
void context_switch(task_struct *proc);
void proc_init(task_struct* proc, uint64_t entry, uint64_t stack_top);

