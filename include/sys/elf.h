#include <sys/defs.h>

#define Elf64_Addr      uint64_t 
#define Elf64_Off       uint64_t 
#define Elf64_Half      uint16_t 
#define Elf64_Word      uint32_t 
#define Elf64_Xword     uint64_t 

#define EI_NIDENT       16

typedef struct
{
    unsigned char e_ident[EI_NIDENT];      /* Magic number and other info */
    Elf64_Half    e_type;                 /* Object file type */
    Elf64_Half    e_machine;              /* Architecture */
    Elf64_Word    e_version;              /* Object file version */
    Elf64_Addr    e_entry;                /* Entry point virtual address */
    Elf64_Off     e_phoff;                /* Program header table file offset */
    Elf64_Off     e_shoff;                /* Section header table file offset */
    Elf64_Word    e_flags;                /* Processor-specific flags */
    Elf64_Half    e_ehsize;               /* ELF header size in bytes */
    Elf64_Half    e_phentsize;            /* Program header table entry size */
    Elf64_Half    e_phnum;                /* Program header table entry count */
    Elf64_Half    e_shentsize;            /* Section header table entry size */
    Elf64_Half    e_shnum;                /* Section header table entry count */
    Elf64_Half    e_shstrndx;             /* Section header string table index */
} Elf64_Ehdr;

typedef struct
{
    Elf64_Word    sh_name;                /* Section name (string tbl index) */
    Elf64_Word    sh_type;                /* Section type */
    Elf64_Xword   sh_flags;               /* Section flags */
    Elf64_Addr    sh_addr;                /* Section virtual addr at execution */
    Elf64_Off     sh_offset;              /* Section file offset */
    Elf64_Xword   sh_size;                /* Section size in bytes */
    Elf64_Word    sh_link;                /* Link to another section */
    Elf64_Word    sh_info;                /* Additional section information */
    Elf64_Xword   sh_addralign;           /* Section alignment */
    Elf64_Xword   sh_entsize;             /* Entry size if section holds table */
} Elf64_Shdr;


typedef struct
{
    Elf64_Word    p_type;                 /* Segment type */
    Elf64_Word    p_flags;                /* Segment flags */
    Elf64_Off     p_offset;               /* Segment file offset */
    Elf64_Addr    p_vaddr;                /* Segment virtual address */
    Elf64_Addr    p_paddr;                /* Segment physical address */
    Elf64_Xword   p_filesz;               /* Segment size in file */
    Elf64_Xword   p_memsz;                /* Segment size in memory */
    Elf64_Xword   p_align;                /* Segment alignment */
} Elf64_Phdr;

//default not more than 16 arguments
#define MAX_ARG         10
#define MAX_ARG_LEN     100

char args[MAX_ARG][MAX_ARG_LEN];

uint64_t check_elf(Elf64_Ehdr* header);
void store_proc_args(task_struct *proc, int arg_cnt);
uint64_t load_phdr(Elf64_Ehdr* hdr, Elf64_Phdr* phdr, task_struct *proc, uint64_t cur_pgd_t);
void stack_heap_vma_allocator(task_struct *proc, uint64_t cur_pgd_t, uint64_t max_addr);
void load_args(task_struct *proc, char *file_name, char *arg_val[]);
task_struct* load_elf(Elf64_Ehdr* hdr, task_struct *proc, char *file_name, char *arg_val[]);
task_struct* load_proc(char* file_name, char *arg_val[]);

