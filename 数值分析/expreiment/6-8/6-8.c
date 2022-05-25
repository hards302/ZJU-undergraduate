#include <stdio.h>
#include <math.h>

double f0( double x, double l, double t )
{
    return sqrt(1.0+l*l*t*t*cos(t*x)*cos(t*x));
}

double Integral(double a, double b, double (*f)(double x, double y, double z), 
double eps, double l, double t);

int main()
{
    double a=0.0, b, eps=0.005, l, t;

    scanf("%lf %lf %lf", &l, &b, &t);
    printf("%.2f\n", Integral(a, b, f0, eps, l, t));

    return 0;
}

/* Your function will be put here */
double Integral(double a, double b, double (*f)(double x, double y, double z), double eps, double l, double t) {
    double h=(b-a)/10000;
    int i;
    double sum = 0.0,x = a;
    for(i = 0; i < 10000; i++) {
        sum += (f0(x,l,t)+f0(x+h,l,t))*h/2;
        x = x+h;
    }
    return sum/100.0;
}