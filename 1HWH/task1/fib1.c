#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "useless.h"

int fib(int n)
{
    return (n == 1 || n == 2) ? 1 : fib(n - 1) + fib(n - 2);
}

int Get_int(char *str)
{
    int len = strlen(str);
    int res = 0;
    for (int i = 0; i < len; i++)
    {
        res += str[i] - '0';
        res *= 10;
    }
    res /= 10;
    return res;
}

int main(char argc, char *argv[])
{
    int n = Get_int(argv[1]);
    printf("%dth number of fibonachi: %d\n", n, fib(n));
    return 0;
}