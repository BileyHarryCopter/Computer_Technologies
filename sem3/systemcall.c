#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main (void)
{
    int value = 1;
    for (int i = 0; i < 5; i++)
    {
        fork();
        printf ("PID: %d PPID: %d \n", getpid(), getppid());
    }
    
    return 0;
}