#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    int index = 0;
    while(s[index] != '\0') {
        sbi_ecall(0x1, 0x0, s[index], 0, 0, 0, 0, 0);
        index++;
    }
}

void puti(int x) {
    // unimplemented
    int temp = x, temp2 = x;
    int cnt = 1;
    while(temp != 0) {
        temp /= 10;
        cnt *= 10;
    }
    cnt /= 10;
    char ch;
    if(x < 0) {
        ch = '-';
        sbi_ecall(0x1, 0x0, ch, 0, 0, 0, 0, 0);
    }
    while(x) {
        temp2 = x;
        int num = temp2 / cnt;
        ch = num + '0';
        sbi_ecall(0x1, 0x0, ch, 0, 0, 0, 0, 0);
        x %= cnt;
        cnt /= 10;
    }
}
