#include <stdio.h>
#include <math.h>

#define MAX_SIZE 10
#define bound pow(2, 127)
#define ZERO 1e-9 /* X is considered to be 0 if |X|<ZERO */

enum bool { false = 0, true = 1 };
#define bool enum bool

int Jacobi( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN );

int Gauss_Seidel( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN );

int main()
{
    int n, MAXN, i, j, k;
    double a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE], x[MAX_SIZE];
    double TOL;

    scanf("%d", &n);
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++)
            scanf("%lf", &a[i][j]);
        scanf("%lf", &b[i]);
    }
    scanf("%lf %d", &TOL, &MAXN);

    printf("Result of Jacobi method:\n");
    for ( i=0; i<n; i++ )
        x[i] = 0.0;
    k = Jacobi( n, a, b, x, TOL, MAXN );
    switch ( k ) {
        case -2:
            printf("No convergence.\n");
            break;
        case -1: 
            printf("Matrix has a zero column.  No unique solution exists.\n");
            break;
        case 0:
            printf("Maximum number of iterations exceeded.\n");
            break;
        default:
            printf("no_iteration = %d\n", k);
            for ( j=0; j<n; j++ )
                printf("%.8f\n", x[j]);
            break;
    }
    printf("Result of Gauss-Seidel method:\n");
    for ( i=0; i<n; i++ )
        x[i] = 0.0;
    k = Gauss_Seidel( n, a, b, x, TOL, MAXN );
    switch ( k ) {
        case -2:
            printf("No convergence.\n");
            break;
        case -1: 
            printf("Matrix has a zero column.  No unique solution exists.\n");
            break;
        case 0:
            printf("Maximum number of iterations exceeded.\n");
            break;
        default:
            printf("no_iteration = %d\n", k);
            for ( j=0; j<n; j++ )
                printf("%.8f\n", x[j]);
            break;
    }

    return 0;
}

/* Your function will be put here */
void swapaii(int n, double a[], double b[]) {
    double c[MAX_SIZE];
    int i;
    for(i = 0; i < n; i++) {
        c[i] = b[i];
        b[i] = a[i];
        a[i] = c[i];
    }
}
void addaii(int n, double a[], double b[]) {
    int i;
    for(i = 0; i < n; i++) a[i] += b[i];
}
void giveval(int n, double a[], double b[]) {
    int i = 0;
    for(i = 0; i < n; i++) a[i] = b[i];
}
double findmax(int a, int b, double c[]) {
    double d = 0.0;
    int i;
    for(i = a; i <= b; i++) {
        if(fabs(c[i]) > d) d = fabs(c[i]);
    }
    return d;
}
double Sum1(int n, int i, int j, double a[][MAX_SIZE], double c[]) {
    double sum = 0.0;
    for(; j < n; j++) {
        if(j == i) continue;
        sum += a[i][j]*c[j];
    }
    return sum;
}
double Sum2(int i, int n, double a[][MAX_SIZE], double c1[], double c2[]) {
    double sum = 0.0;
    int j;
    for(j = 0; j < i; j++) {
        sum += a[i][j]*c1[j];
    }
    for(j = i+1; j < n; j++) {
        sum += a[i][j]*c2[j];
    }
    return sum;
}
int domat(int n, double a[][MAX_SIZE], double b[]) {
    int i, j, k, f;
    double d = 0.0;
    for(j = 0; j < n; j++) {
        f = 0;
        d = 0.0;
        for(i = j; i < n; i++) {
            if(a[i][j] != 0) f = 1;
            if(fabs(a[i][j]) > d) {
                d = fabs(a[i][j]);
                k = i;
            }
        }
        if(f == 0) return 0;
        if(a[k][j] != 0) {
            swapaii(n, a[j], a[k]);
            f = b[k]; b[k] = b[j]; b[j] = f;
        }
        else {
            d = 0.0;
            for(i = k-1; i >= 0; i--) {
                if(fabs(a[i][j]) > d) {
                    d = fabs(a[i][j]);
                    k = i;
                }
            }
            addaii(n, a[j], a[k]);
            b[j] += b[k];
        }
    }
    return 1;
}
int Jacobi( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN ) {
    if(domat(n, a, b) == 0) return -1;
    int i,j,k;
    double X[MAX_SIZE], cha[MAX_SIZE];
    for(k = 0; k < MAXN; k++) {
        for(i = 0; i < n; i++) X[i] = (-Sum1(n, i, 0, a, x) + b[i])/a[i][i];
        if(findmax(0,n-1,X) > bound) return -2;
        for(j = 0; j < n; j++) cha[j] = fabs(x[j]-X[j]);
        if(findmax(0, n-1, cha) < TOL) return k+1;
        giveval(n,x,X);
    }
    return 0;
}
int Gauss_Seidel( int n, double a[][MAX_SIZE], double b[], double x[], double TOL, int MAXN ) {
    if(domat(n, a, b) == 0) return -1;
    int i,j,k;
    double X[MAX_SIZE], cha[MAX_SIZE];
    for(k = 0; k < MAXN; k++) {
        for(i = 0; i < n; i++) X[i] = (-Sum2(i, n, a, X, x) + b[i])/a[i][i];
        if(findmax(0,n-1,X) > bound) return -2;
        for(j = 0; j < n; j++) cha[j] = fabs(x[j]-X[j]);
        if(findmax(0, n-1, cha) < TOL) return k+1;
        giveval(n,x,X);
    }
    return 0;
}