.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15HandleInterruptEhj

.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
  movb $\num, (interruptnumber)
  jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
  movb $\num + IRQ_BASE, (interruptnumber)
  jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:
    # save registers
    #pusha
    #pushl %ds
    #pushl %es
    #pushl %fs
    #pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    # call C++ Handler
    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15HandleInterruptEhj
    mov %eax, %esp # switch the stack

    # restore registers
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

    #pop %gs
    #pop %fs
    #pop %es
    #pop %ds
    #popa

    add $4, %esp

.global _ZN16InterruptManager15InterruptIgnoreEv
_ZN16InterruptManager15InterruptIgnoreEv:

  iret

.data
  interruptnumber: .byte 0
