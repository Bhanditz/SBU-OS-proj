.text

######
# load a new IDT
#  parameter: address of lidt
.global _x86_64_asm_lidt
_x86_64_asm_lidt:

    lidt (%rdi)
    
    retq