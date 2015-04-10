.macro push_al
	pushq 		%rax
    pushq 		%rbx
    pushq 		%rcx
    pushq 		%rdx
    pushq 		%rdi
    pushq 		%rsi
    pushq 		%rsp
    pushq 		%rbp
    pushq 		%r8
    pushq 		%r9
    pushq 		%r10
    pushq 		%r11
    pushq 		%r12
    pushq 		%r13
    pushq 		%r14
    pushq 		%r15
.endm

.macro pop_al
	popq		%r15
    popq		%r14
    popq		%r13
    popq		%r12
    popq		%r11
    popq		%r10
    popq		%r9
    popq		%r8
    popq		%rbp
    popq		%rsp
    popq		%rsi
    popq		%rdi
    popq		%rdx
    popq		%rcx
    popq		%rbx
    popq		%rax
.endm

.globl   isr_timer
isr_timer:
	cli
   push_al
   call 		timer_handler
	pop_al
	sti
   iretq

.globl   isr_keyboard
isr_keyboard:
   cli
   push_al
   call     keyboard_handler
   pop_al
   sti
   iretq




#general interrupt handler

.globl   isr0
isr0:
   cli
   push_al
   call        isr_0
   pop_al
   sti
   iretq

.globl   isr1
isr1:
   cli
   push_al
   call        isr_1
   pop_al
   sti
   iretq

.globl   isr2
isr2:
   cli
   push_al
   call        isr_2
   pop_al
   sti
   iretq

.globl   isr3
isr3:
   cli
   push_al
   call        isr_3
   pop_al
   sti
   iretq

.globl   isr4
isr4:
   cli
   push_al
   call        isr_4
   pop_al
   sti
   iretq

.globl   isr5
isr5:
   cli
   push_al
   call        isr_5
   pop_al
   sti
   iretq

.globl   isr6
isr6:
   cli
   push_al
   call        isr_6
   pop_al
   sti
   iretq

.globl   isr7
isr7:
   cli
   push_al
   call        isr_7
   pop_al
   sti
   iretq

.globl   isr8
isr8:
   cli
   push_al
   call        isr_8
   pop_al
   sti
   iretq

.globl   isr9
isr9:
   cli
   push_al
   call        isr_9
   pop_al
   sti
   iretq

.globl   isr10
isr10:
   cli
   push_al
   call        isr_10
   pop_al
   sti
   iretq

.globl   isr11
isr11:
   cli
   push_al
   call        isr_11
   pop_al
   sti
   iretq

.globl   isr12
isr12:
   cli
   push_al
   call        isr_12
   pop_al
   sti
   iretq

.globl   isr13
isr13:
   cli
   push_al
   call        isr_13
   pop_al
   sti
   iretq

.globl   isr14
isr14:
   cli
   push_al
   call        isr_14
   pop_al
   sti
   iretq

.globl   isr15
isr15:
   cli
   push_al
   call        isr_15
   pop_al
   sti
   iretq

.globl   isr16
isr16:
   cli
   push_al
   call        isr_16
   pop_al
   sti
   iretq

.globl   isr17
isr17:
   cli
   push_al
   call        isr_17
   pop_al
   sti
   iretq

.globl   isr18
isr18:
   cli
   push_al
   call        isr_18
   pop_al
   sti
   iretq

.globl   isr19
isr19:
   cli
   push_al
   call        isr_19
   pop_al
   sti
   iretq

.globl   isr20
isr20:
   cli
   push_al
   call        isr_20
   pop_al
   sti
   iretq

.globl   isr21
isr21:
   cli
   push_al
   call        isr_21
   pop_al
   sti
   iretq

.globl   isr22
isr22:
   cli
   push_al
   call        isr_22
   pop_al
   sti
   iretq

.globl   isr23
isr23:
   cli
   push_al
   call        isr_23
   pop_al
   sti
   iretq

.globl   isr24
isr24:
   cli
   push_al
   call        isr_24
   pop_al
   sti
   iretq

.globl   isr25
isr25:
   cli
   push_al
   call        isr_25
   pop_al
   sti
   iretq

.globl   isr26
isr26:
   cli
   push_al
   call        isr_26
   pop_al
   sti
   iretq

.globl   isr27
isr27:
   cli
   push_al
   call        isr_27
   pop_al
   sti
   iretq

.globl   isr28
isr28:
   cli
   push_al
   call        isr_28
   pop_al
   sti
   iretq

.globl   isr29
isr29:
   cli
   push_al
   call        isr_29
   pop_al
   sti
   iretq

.globl   isr30
isr30:
   cli
   push_al
   call        isr_30
   pop_al
   sti
   iretq

.globl   isr31
isr31:
   cli
   push_al
   call        isr_31
   pop_al
   sti
   iretq

.globl   isr34
isr34:
   cli
   push_al
   call        isr_34
   pop_al
   sti
   iretq

.globl   isr35
isr35:
   cli
   push_al
   call        isr_35
   pop_al
   sti
   iretq

.globl   isr36
isr36:
   cli
   push_al
   call        isr_36
   pop_al
   sti
   iretq

.globl   isr37
isr37:
   cli
   push_al
   call        isr_37
   pop_al
   sti
   iretq

.globl   isr38
isr38:
   cli
   push_al
   call        isr_38
   pop_al
   sti
   iretq

.globl   isr39
isr39:
   cli
   push_al
   call        isr_39
   pop_al
   sti
   iretq

.globl   isr40
isr40:
   cli
   push_al
   call        isr_40
   pop_al
   sti
   iretq

.globl   isr41
isr41:
   cli
   push_al
   call        isr_41
   pop_al
   sti
   iretq

.globl   isr42
isr42:
   cli
   push_al
   call        isr_42
   pop_al
   sti
   iretq

.globl   isr43
isr43:
   cli
   push_al
   call        isr_43
   pop_al
   sti
   iretq

.globl   isr44
isr44:
   cli
   push_al
   call        isr_44
   pop_al
   sti
   iretq

.globl   isr45
isr45:
   cli
   push_al
   call        isr_45
   pop_al
   sti
   iretq

.globl   isr46
isr46:
   cli
   push_al
   call        isr_46
   pop_al
   sti
   iretq

.globl   isr47
isr47:
   cli
   push_al
   call        isr_47
   pop_al
   sti
   iretq

.globl   isr48
isr48:
   cli
   push_al
   call        isr_48
   pop_al
   sti
   iretq

.globl   isr49
isr49:
   cli
   push_al
   call        isr_49
   pop_al
   sti
   iretq

.globl   isr50
isr50:
   cli
   push_al
   call        isr_50
   pop_al
   sti
   iretq

