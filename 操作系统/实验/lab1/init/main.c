#include "print.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    puti(2021);
    puts(" Hello 3190104547\n");

    test(); // DO NOT DELETE !!!

	return 0;
}
