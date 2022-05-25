#include <stdio.h>
#include <math.h>

#define MAX_m 200
#define MAX_n 5

double f1(double x)
{
    return sin(x);
}

double f2(double x)
{
    return exp(x);
}

int OPA( double (*f)(double t), int m, double x[], double w[], double c[], double *eps );

void print_results( int n, double c[], double eps)
{    
    int i;

    printf("%d\n", n);
    for (i=0; i<=n; i++)
        printf("%12.4e ", c[i]);
    printf("\n");
    printf("error = %9.2e\n", eps);
    printf("\n");
}

int main()
{
    int m, i, n;
    double x[MAX_m], w[MAX_m], c[MAX_n+1], eps;

    m = 90;
    for (i=0; i<m; i++) {
        x[i] = 3.1415926535897932 * (double)(i+1) / 180.0;
        w[i] = 1.0;
    }
    eps = 0.001;
    n = OPA(f1, m, x, w, c, &eps);
    print_results(n, c, eps);

    m = 200;
    for (i=0; i<m; i++) {
        x[i] = 0.01*(double)i;
        w[i] = 1.0;
    }
    eps = 0.001;
    n = OPA(f2, m, x, w, c, &eps);
    print_results(n, c, eps);

    return 0;
}

/* Your function will be put here */
double nj(double w[],double x[],double v[],int m) {
    int i;
    double nj = 0.0;
    for(i = 0; i < m; i++) nj += w[i]*x[i]*v[i];
    return nj;
}
void prodc(double s[], double p, double h[], int m, double w[]) {
    int i,j;
    for(i = 0; i < 12; i++) {
        for(j = 0; j < 6; j++) printf("w[%d] = %f ",j,w[j]); printf("\n");
        s[i] = p*h[i];
    }
}
void prodx(double j[], double l[], int m) {
    int i;
    for(i = m-1; i > 0; i--) j[i] = l[i-1];
    j[0] = 0;
}
void addv(double c[], double s[], int m) {
    int i;
    for(i = 0; i < m; i++) c[i] += s[i];
}
void givev(double f1[], double f2[], int m) {
    int i;
    for(i = 0; i < m; i++) f1[i] = f2[i];
}
void workf(double (*f)(double t), int m, double x[], double v[]) {
    int i = 0;
    for(i = 0; i < m; i++) v[i] = f(x[i]);
}
void calp(double c[], double x, double result, int m) {
    for(int i = 0; i < m; i++) {
        result += c[i]*pow(x,i);
    }
}
int OPA( double (*f)(double t), int m, double x[], double w[], double c[], double *eps ) {
    double v[MAX_m],fi0[MAX_m],fi1[MAX_m],fi2[MAX_m],a[MAX_m],y[MAX_m],B[MAX_m],C[MAX_m],temp[MAX_m],temp2[MAX_m],temp3[MAX_m];
    double err = 0.0;
    for(int i = 0; i < m; i++) {
        v[i]=0;fi0[i]=0;fi1[i]=0;fi2[i]=0;a[i]=0;y[i]=0;B[i]=0;C[i]=0;temp[i]=0;temp2[i]=0;temp3[i]=0;
    }
    int k = 1;
    workf(f,m,x,v);
    fi0[0] = 1;
    a[0] = nj(w,fi0,v,m)/nj(w,fi0,fi0,m);
    prodc(c,a[0],fi0,m,w);
    for(int i = 0; i < m; i++) w[i] = 1;
    err = nj(w,v,v,m)-a[0]*nj(w,fi0,v,m);
    prodx(temp,fi0,m);
    B[1] = nj(w,temp,fi0,m); fi1[1] = 1; fi1[0] = -B[1];
    a[1] = nj(w,fi1,v,m)/nj(w,fi1,fi1,m);
    /*prodc(temp,a[1],fi1,m);
    addv(c,temp,m);
    for(int i = 0; i < m; i++) {
        double cp;
        calp(c,x[i],cp,m);
        err += w[i]*(f(x[i])-cp)*(f(x[i])-cp);
    }
    while(k < MAX_n && fabs(err) >= *eps) {
        k++;
        prodx(temp,fi1,m);
        B[k] = nj(w,temp,fi1,m)/nj(w,fi1,fi1,m);
        C[k] = nj(w,temp,fi0,m)/nj(w,fi0,fi0,m);
        prodx(temp,fi1,m);
        prodc(temp2,-B[k],fi1,m);
        prodc(temp3,-C[k],fi0,m);
        addv(temp, temp2, m);
        addv(temp, temp3, m);
        givev(fi2,temp,m);
        a[k] = nj(w,fi2,v,m)/nj(w,fi2,fi2,m);
        prodc(temp,a[k],fi2,m);
        addv(c,temp,m);
        for(int i = 0; i < m; i++) {
            double cp;
            calp(c,x[i],cp,m);
            err += w[i]*(f(x[i])-cp)*(f(x[i])-cp);
        }
        givev(fi0,fi1,m); givev(fi1,fi2,m);
    }*/
    *eps = err;
    return k-1;
}