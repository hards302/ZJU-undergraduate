#include <stdio.h>
#include <math.h>

#define ZERO 1e-13 /* X is considered to be 0 if |X|<ZERO */
#define MAXN 11    /* Max Polynomial Degree + 1 */

double Polynomial_Root(int n, double c[], double a, double b, double EPS);

int main()
{
    int n;
    double c[MAXN], a, b;
    double EPS = 0.00005;
    int i;

    scanf("%d", &n);
    for (i=n; i>=0; i--) 
        scanf("%lf", &c[i]);
    scanf("%lf %lf", &a, &b);
    printf("%.4f\n", Polynomial_Root(n, c, a, b, EPS));

    return 0;
}

/* Your function will be put here */
double res(int n, double c[], double m) {
    double result = c[n];
    int i;
    for(i = n; i > 0; i--) {
        result = result*m + c[i-1];
    }    
    return result;
}
double deri(int n, double c[], double m) {    //求一阶导数
    double cc[MAXN];
    int i;
    for(i = n; i > 0; i--) cc[i-1] = i*c[i];
    return res(n-1, cc, m);
}
double deri2(int n, double c[], double m) {   //求二阶导数
    double cc[MAXN];
    int i;
    for(i = n; i > 1; i--) cc[i-2] = i*(i-1)*c[i];
    return res(n-2, cc, m);
}
double Polynomial_Root(int n, double c[], double a, double b, double EPS) {
    double mid = (a+b)/2.0;
    double der,der2;
    double pre;
    int i;
    if(a > b) {
        int c = a;
        a = b;
        b = c;
    }
    if(res(n,c,a) == 0) return a;
    if(res(n,c,b) == 0) return b;
    der = deri(n, c, mid);
    der2 = deri2(n, c, mid);
    /*while(fabs(der*der) < fabs(res(n,c,mid))*der2/2) {   //初始值
        if(res(n,c,mid) * res(n,c,a) <= 0) mid = (mid+a)/2.0;
        else mid = (mid+b)/2.0;
        der = deri(n, c, mid);
        der2 = deri2(n, c, mid);
    }*/
    for(i = 0; i < 10; i++) {
        if(mid * a <= 0) mid = (mid+a)/2.0;
        else mid = (mid+b)/2.0;
    }
    do{
        pre = mid;
        der = deri(n,c,pre);
        der2 = deri2(n,c,pre);
        if(der == 0) {
            if(res(n,c,a) * res(n,c,pre) <= 0) mid = (a+pre)/2.0;
            else mid = (b+pre)/2.0;
        }
        else {
            mid = pre - res(n,c,pre)/der;
            /*for(i = 0; mid < a || mid > b; i++) {        //出区间
                mid = pre - res(n,c,pre)/(pow(2,i)*der);
            }*/
        }
    }while(fabs(pre-mid) >= EPS + ZERO);
    if(mid == -0.0) mid = 0.0;
    return mid;
}