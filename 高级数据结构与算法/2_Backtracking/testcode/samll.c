#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main() {
	freopen("./test.txt", "w", stdout);
	srand(time(NULL));
	int n = rand()%30;
	int m = n+rand()%(30-n);
	printf("%d %d\n", n, m);
	for(int i = 0; i < n; i++) {
		int a = rand()%n+1;
		int b = rand()%n+1;
		printf("%03d %03d\n", a, b);
	}
	for(int i = 0; i < m; i++) {
		int a = m-i;
		int b = rand()%1000;
		printf("%03d %d\n", a, b);
	}
	return 0;
}