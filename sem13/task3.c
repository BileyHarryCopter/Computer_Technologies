#include <stdio.h>
#include <signal.h>

#define start_old 5
static int com_count = 0;

void handler_old(int sig)
{
    signal(sig, SIG_IGN);
}

void handler_new(int sig)
{
    if (com_count == start_old)
    {
        signal(sig, handler_old);
        return;
    }

    if (sig == SIGINT)
        printf("SIGINT is given\n");
    else if (sig == SIGQUIT)
        printf("SIGQUIT is given\n");
    else
        printf("Unknown signal\n");

    ++com_count;
}

int main()
{
    while (1)
    {
        signal(SIGINT, handler_new);
    }
    return 0;
}