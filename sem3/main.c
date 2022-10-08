#include <stdio.h>
#include <unistd.h>
int main (int argc, char* argv[], char* envp[])
{
    int status = fork();
    if (status == -1)
        printf("Fuck you\n");
    if (status == 0)
    {
        printf("In the process (PID: %d, PPID: %d). Yeah, we are in the child\n", getpid(), getppid());
        sleep(2);
    }
    else
    {
        printf("In the process (PID: %d, PPID: %d). Grandma, aren't you?\n", getpid(), getppid());
        sleep(2);
    }
    return 0;
}