#include "printk.h"
#include "clock.h"
void trap_handler(unsigned long long scause, unsigned long long sepc) {
    // 通过 `scause` 判断trap类型  interrupt: scause[31] = 1
    // 如果是interrupt 判断是否是timer interrupt : scause[3:0] = 101
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略
    // printk("scause = %x, sepc = %x\n", scause, sepc);
    printk("kernel is running~\n");
    if((scause >> 63 == 1) & (scause & 0b111)==5) {
        printk("[S] Supervisor Mode Timer Interrupt!\n");
        clock_set_next_event();
    }
}