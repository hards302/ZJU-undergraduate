.extern start_kernel
.extern _traps

    .section .text.init
    .globl _start
_start:

    la t0, _traps
    csrw 0x105, t0    # stvec = _traps

    add t1, x0, 32
    csrs 0x104, t1    # sie[stie] = 1
    
    # set time interrupt. ecall(0,0,time,0,0,0,0,0)
    mv a7, x0
    mv a6, x0
    rdtime a0            # get time now
    li t1, 10000000      # TIMECLOCK = 10MHz
    add a0, a0, t1
    mv a1, x0
    mv a2, x0
    mv a3, x0
    mv a4, x0
    mv a5, x0
    ecall

    addi t1, x0, 2
    csrs 0x100, t1       # sstatus[sie] = 1

    # ------------------
    # - your code here -
    # ------------------
    la sp, boot_stack_top
    jal x0, start_kernel

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 1024 * 4 # 4K space

    .globl boot_stack_top
boot_stack_top:
    # end of stack