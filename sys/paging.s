.text

######
# load pml4 physical address to CR3
# parameter: address of pml4 kernel table
.global load_cr3
load_cr3:

    movq %rdi, %rax

    movq %rax, %cr3

    retq
