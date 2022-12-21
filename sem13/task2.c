#include <signal.h>
#include <stdio.h>

void handler(int sig)
{
    if (sig == SIGINT)
        printf("SIGINT is given\n");
    else if (sig == SIGQUIT)
        printf("SIGQUIT is given\n");
    else
        printf("Unknown signal\n");
    return;
}

int main()
{
    while (1)
    {
        signal(SIGINT, handler);
        signal(SIGQUIT, handler);
    }
    return 0;
}