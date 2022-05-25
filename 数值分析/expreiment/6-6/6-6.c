#include <stdio.h>

#define MAX_N 10

void Cubic_Spline(int n, double x[], double f[], int Type, double s0, double sn, double a[], double b[], double c[], double d[]);

double S( double t, double Fmax, int n, double x[], double a[], double b[], double c[], double d[] );

int main()
{
    int n, Type, m, i;
    double x[MAX_N], f[MAX_N], a[MAX_N], b[MAX_N], c[MAX_N], d[MAX_N];
    double s0, sn, Fmax, t0, tm, h, t;

    scanf("%d", &n);
    for (i=0; i<=n; i++) 
        scanf("%lf", &x[i]);
    for (i=0; i<=n; i++) 
        scanf("%lf", &f[i]);
    scanf("%d %lf %lf %lf", &Type, &s0, &sn, &Fmax);

    Cubic_Spline(n, x, f, Type, s0, sn, a, b, c, d);
    for (i=1; i<=n; i++)
        printf("%12.8e %12.8e %12.8e %12.8e \n", a[i], b[i], c[i], d[i]);

    scanf("%lf %lf %d", &t0, &tm, &m);
    h = (tm-t0)/(double)m;
    for (i=0; i<=m; i++) {
        t = t0+h*(double)i;
        printf("f(%12.8e) = %12.8e\n", t, S(t, Fmax, n, x, a, b, c, d));
    }

    return 0;
}

/* Your functions will be put here */
#include<math.h>
double a0,b0,c0,d0;
void Cubic_Spline(int n, double x[], double f[], int Type, double s0, double sn, double a[], double b[], double c[], double d[]){
    int i,j;
    double h[MAX_N],arf[MAX_N],l[MAX_N],z[MAX_N],miu[MAX_N];
    for(i = 0; i <= n; i++) a[i] = f[i];
    if(Type == 1) {
        for(i = 0; i < n; i++) h[i] = x[i+1]-x[i];
        arf[0] = 3*(a[1]-a[0])/h[0]-3*s0;
        arf[n] = 3*sn-3*(a[n]-a[n-1])/h[n-1];
        for(i = 1; i < n; i++) arf[i] = 3*(a[i+1]-a[i])/h[i]-3*(a[i]-a[i-1])/h[i-1];
        l[0] = 2*h[0];
        miu[0] = 0.5;
        z[0] = arf[0]/l[0];
        for(i = 1; i < n; i++) {
            l[i] = 2*(x[i+1]-x[i-1])-h[i-1]*miu[i-1];
            miu[i] = h[i]/l[i];
            z[i] = (arf[i]-h[i-1]*z[i-1])/l[i];
        }
        l[n] = h[n-1]*(2-miu[n-1]);
        z[n] = (arf[n]-h[n-1]*z[n-1])/l[n];
        c[n] = z[n];
        for(j = n-1; j >= 0; j--) {
            c[j] = z[j]-miu[j]*c[j+1];
            b[j] = (a[j+1]-a[j])/h[j]-h[j]*(c[j+1]+2*c[j])/3;
            d[j] = (c[j+1]-c[j])/h[j]/3;
        }
    }
    else if(Type == 2) {
        for(i = 0; i < n; i++) h[i] = x[i+1]-x[i];
        for(i = 1; i < n; i++) arf[i] = 3*(a[i+1]-a[i])/h[i]-3*(a[i]-a[i-1])/h[i-1];
        l[0] = 1; miu[0] = 0; z[0] = 0;
        for(i = 1; i < n; i++) {
            l[i] = 2*(x[i+1]-x[i-1]) - h[i-1]*miu[i-1];
            miu[i] = h[i]/l[i];
            z[i] = (arf[i]-h[i-1]*z[i-1])/l[i];
        }
        l[n] = 1;
        z[n] = s0;
        c[n] = sn;
        for(j = n-1; j >= 0; j--) {
            c[j] = z[j]-miu[j]*c[j+1];
            b[j] = (a[j+1]-a[j])/h[j] - h[j]*(c[j+1]+2*c[j])/3;
            d[j] = (c[j+1]-c[j])/h[j]/3;
        }
    }
    a0 = a[n], b0 = b[n], c0 = c[n], d0 = d[n];
    for(i = n; i >= 1; i--) {
        a[i] = a[i-1];
        b[i] = b[i-1];
        c[i] = c[i-1];
        d[i] = d[i-1];
    }
}

double S( double t, double Fmax, int n, double x[], double a[], double b[], double c[], double d[] ) {
    double sum = 0;
    int j;
    if(t < x[0]-Fmax || t > x[n]+Fmax) return 0;
    for(j = 0; j < n; j++) {
        if(t >= x[j] && t < x[j+1]) {
            sum = a[j+1]+b[j+1]*(t-x[j])+c[j+1]*pow((t-x[j]),2)+d[j+1]*pow((t-x[j]),3);
        } 
    }
    if(t == x[n]) sum = a0+b0*(t-x[n])+c0*pow((t-x[n]),2)+d0*pow((t-x[n]),3);
    return sum;
}