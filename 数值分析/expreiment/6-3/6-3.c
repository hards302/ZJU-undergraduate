#include <stdio.h>
#include <math.h>
#define Max_size 10000 /* max number of dishes */

void Price( int n, double p[] );

int main()
{
    int n, i;
    double p[Max_size];

    scanf("%d", &n);
    for (i=0; i<n; i++) 
        scanf("%lf", &p[i]);
    Price(n, p);
    for (i=0; i<n; i++)
        printf("%.2f ", p[i]);
    printf("\n");

    return 0;
}

/* Your function will be put here */
double findmax(int a, int b, double c[]) {
    double d = 0.0;
    int i;
    for(i = a; i <= b; i++) {
        if(fabs(c[i]) > d) d = fabs(c[i]);
    }
    return d;
}
void giveval(int n, double a[], double b[]) {
    int i = 0;
    for(i = 0; i < n; i++) a[i] = b[i];
}
void Price(int n, double p[]) {
    int i,j,k;
    double X[Max_size], x[Max_size], cha[Max_size];
    for(i = 0; i < n; i++) {X[i] = 0.0;x[i] = 0.0;}
    for(k = 0; k < 1000; k++) {
        X[0] = (-(0.5*x[1]+0.5*x[n-1])+p[0])/2.0;
        for(i = 1; i < n-1;i++) X[i] = (-(0.5*x[i-1]+0.5*x[i+1])+p[i])/2.0;
        X[n-1] = (-(0.5*x[0]+0.5*x[n-2])+p[n-1])/2.0;
        for(j = 0; j < n; j++) cha[j] = fabs(x[j]-X[j]);
        if(findmax(0,n-1,cha) < 0.001) {giveval(n,p,x); return;}
        giveval(n,x,X);
    }
}