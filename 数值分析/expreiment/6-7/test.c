#include <stdio.h>
#include <math.h>
double nj(double a[],double b[],double w[],int n) {
    int i;
    double nj = 0.0;
    for(i = 0; i < n; i++) nj += w[i]*a[i]*b[i];
    return nj;
}
double f[5];
double w[5] = {1,1,1,1,1};
int main() {
    f[0] = 1;
    printf("%f",nj(f,f,w,5));
    return 0;
}