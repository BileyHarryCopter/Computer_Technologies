#include <stdio.h>
#include "useless.h"

int main(char argc, char *argv[])
{
    if (argc != 2)
    {
        printf("And where is file with executable programs?\n");
        return 0;
    }
    execution(argv[1]);
    return 0;
}