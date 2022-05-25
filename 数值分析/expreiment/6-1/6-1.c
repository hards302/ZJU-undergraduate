#include <stdio.h>

void Series_Sum( double sum[] );

int main()
{
    int i;
    double x, sum[3001];
	
    Series_Sum( sum );

    x = 0.0;
    for (i = 0; i < 11; i++)
        printf("%6.2f %16.12f\n", x + (double)i * 0.10, sum[i]);

    return 0;
}

/* Your function will be put here */
void Series_Sum( double sum[] ) {
    int k;
    int i;
    double x,Sum;
    /*for(i = 0; i < 11; i++) {
        x = i*0.1;
        sum[i] = 0;
        for(k = 1; k < 2000; k++) {
            sum[i] += 1.0/(k*(k+1)*(k+2)*(k+x));
        }
        sum[i] = (sum[i]*(2-x)+0.25)*(1-x) + 1.0;
    }
    for(i = 11; i < 3001; i++) {
        double x = i*0.1;
        sum[i] = ((x-1.0)*sum[i-10]+1.0/x)/x;
    }*/
    for(i = 0; i < 11; i++) {
        x = i*0.1;
        Sum = 0;
        for(k = 1; k <= 300000; k++) {
            Sum += 1.0/(k*(k+x)*(k+1));
        }
        sum[i] = Sum * (1-x) + 1.0;
    }
    for(i = 11; i < 3001; i++) {
        double x = i*0.1;
        sum[i] = ((x-1.0)*sum[i-10]+1.0/x)/x;
    }
}