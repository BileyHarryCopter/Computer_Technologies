#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int fib(int n)
{
    return (n == 1 || n == 2) ? 1 : fib(n - 1) + fib(n - 2);
}

int main(char argc, char *argv[])
{
    int n;
    scanf("%d", &n);
    printf("%dth number of fibonachi: %d\n", n, fib(n));
    return 0;
}