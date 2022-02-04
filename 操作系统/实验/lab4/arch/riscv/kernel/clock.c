#include "sbi.h"
#include "printk.h"

unsigned long TIMECLOCK = 10000000;

unsigned long get_cycles() {
    unsigned long t;
    __asm__ volatile(

		"rdtime %[t]\n"
		: [t] "=r" (t)
		: 
		: "memory"
	);
    return t;
}

void clock_set_next_event() {
    unsigned long next = get_cycles() + TIMECLOCK;
	// printk("now = %lu, next = %lu, timeclock = %lu\n",get_cycles(), next, TIMECLOCK);
    sbi_ecall(0,0,next,0,0,0,0,0);
}