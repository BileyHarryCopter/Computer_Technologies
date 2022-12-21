#include <signal.h>

int main()
{
    while (1)
    {
        signal(SIGINT, SIG_IGN);
    }
    return 0;
}