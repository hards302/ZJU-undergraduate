#include <stdio.h>

#define MAX_SIZE 10

int EigenV(int n, double a[][MAX_SIZE], double *lambda, double v[], double TOL, int MAXN);

int main()
{
    int n, MAXN, m, i, j, k;
    double a[MAX_SIZE][MAX_SIZE], v[MAX_SIZE];
    double lambda, TOL;

    scanf("%d", &n);
    for (i=0; i<n; i++) 
        for (j=0; j<n; j++) 
            scanf("%lf", &a[i][j]);
    scanf("%lf %d", &TOL, &MAXN);
    scanf("%d", &m);
    for (i=0; i<m; i++) {
        scanf("%lf", &lambda);
        for (j=0; j<n; j++)
            scanf("%lf", &v[j]);
        switch (EigenV(n, a, &lambda, v, TOL, MAXN)) {
            case -1: 
                printf("%12.8f is an eigenvalue.\n", lambda );
                break;
            case 0:
                printf("Maximum number of iterations exceeded.\n");
                break;
            case 1:
                printf("%12.8f\n", lambda );
                for (k=0; k<n; k++)
                    printf("%12.8f ", v[k]);
                printf("\n");
                break;
        }
    }

    return 0;
}

/* Your function will be put here */
#include<math.h>
double findinf(int n, double a[]) {
    int i;
    double inf = 0.0;
    for(i = 0; i < n; i++) {
        if(fabs(a[i]) > inf) inf = fabs(a[i]);
    }
    return inf;
}
int findpos(int n, double a[]) {
    int i, pos = 0;
    for(i = 0; i < n; i++) {
        if(fabs(a[i]) > fabs(a[pos])) pos = i;
    }
    return pos;
}
double A[MAX_SIZE][MAX_SIZE];     
double l[MAX_SIZE][MAX_SIZE];
double u[MAX_SIZE][MAX_SIZE];
double y[MAX_SIZE];
double c[MAX_SIZE];
int calmatrix(double A[][MAX_SIZE], double x[], double b[], int n) {
    int i, j, k;
    for(i = 0; i < n; i++) {
        l[i][i] = 1;
        for(j = i; j < n; j++) {
            double sum = 0;
            for(k = 0; k < i; k++) sum += l[i][k]*u[k][i];
            u[i][j] = A[i][j]-sum;
        }
        for(j = i+1; j < n; j++) {
            double sum = 0;
            for(k = 0; k < i; k++) sum += l[j][k]*u[k][i];
            if(u[i][i] == 0) return -1;
            l[j][i] = (A[j][i] - sum) / u[i][i];
        }
    }
    y[0] = b[0];
    for(i = 1; i < n; i++) {
        double sum = 0;
        for(j = 0; j < i; j++) sum += l[i][j]*y[j];
        y[i] = b[i]-sum;
    }
    for(i = n-1; i >= 0; i--) {
        if(u[i][i] == 0) return -1;
        double sum = 0;
        for(j = i+1; j < n; j++) sum += u[i][j]*x[j];
        x[i] = (y[i]-sum)/u[i][i];
    }
}
int EigenV(int n, double a[][MAX_SIZE], double *lambda, double v[], double TOL, int MAXN) {
    int k = 0,i,j;
    int p = findpos(n, v);
    if(v[p] == 0) return -1;
    for(i = 0; i < n; i++) v[i] = v[i]/v[p];
    while(1) {
        if(k > MAXN) return 0;
        double miu,ERR;
        for(i = 0; i < n; i++) {
            for(j = 0; j < n; j++) {
                if(i == j) A[i][i] = a[i][i]-*lambda;
                else A[i][j] = a[i][j];
           }
        }
        if(calmatrix(A, y, v, n) == -1) return -1;
        p = findpos(n, y);
        miu = y[p];
        for(i = 0; i < n; i++) y[i] = y[i]/miu;
        for(i = 0; i < n; i++) c[i] = v[i]-y[i];
        ERR = findinf(n, c);
        for(i = 0; i < n; i++) v[i] = y[i];
        if(ERR < TOL) {
            miu = 1/miu + *lambda;
            *lambda = miu;
            return 1;
        }
        k++;
    }
}