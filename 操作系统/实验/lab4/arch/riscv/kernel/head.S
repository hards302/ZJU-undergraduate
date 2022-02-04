.extern start_kernel
.extern _traps

    .section .text.init
    .globl _start
_start:
    
    call setup_vm
    call relocate

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
    call mm_init             # mm_init after sp init
    call task_init

    call setup_vm_final

    j start_kernel

relocate:
    # set ra = ra + PA2VA_OFFSET
    li t0, 0xffffffdf80000000
    add ra, ra, t0

    # set satp with early_pgtbl
    li t0, 8
    slli t0, t0, 60
    la t1, early_pgtbl
    srli t1, t1, 12
    add t0, t0, t1
    csrw satp, t0

    # flush tlb
    sfence.vma zero, zero
    
    ret

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 1024 * 4 # 4K space

    .globl boot_stack_top
boot_stack_top:
    # end of stack