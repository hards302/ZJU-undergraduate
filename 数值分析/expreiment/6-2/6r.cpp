double res(int n, double c[], double m) {
    double result = c[n];
    int i;
    for(i = n; i > 0; i--) {
        result = result*m + c[i-1];
    }    
    return result;
}
double deri(int n, double c[], double m) {
    double cc[MAXN];
    int i;
    for(i = n; i > 0; i--) cc[i-1] = i*c[i];
    return res(n-1, cc, m);
}
double Polynomial_Root(int n, double c[], double a, double b, double EPS) {
    double mid = (a+b)/2.0;
    double der;
    double pre;
    int i;
    if(a > b) {
        int c = a;
        a = b;
        b = c;
    }
    if(res(n,c,a) == 0) return a;
    if(res(n,c,b) == 0) return b;
    for(i = 0; i < 10; i++) {
        if(mid * a <= 0) mid = (mid+a)/2.0;
        else mid = (mid+b)/2.0;
    }
    do{
        pre = mid;
        der = deri(n,c,pre);
        if(der == 0) {
            if(res(n,c,a) * res(n,c,pre) <= 0) mid = (a+pre)/2.0;
            else mid = (b+pre)/2.0;
        }
        else {
            mid = pre-res(n,c,pre)/deri(n,c,pre);
        }
    }while(fabs(pre-mid) >= EPS + ZERO);
    if(mid == -0.0) mid = 0.0;
    return mid;
}