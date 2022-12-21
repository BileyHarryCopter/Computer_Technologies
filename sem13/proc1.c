#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void handler(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, 0);
    if (pid < 0)
    {
        printf("Failed with getting status. Errno = %d\n", errno);
        return;
    }

    // EXIT
    if ((status & 0xff) == 0)
        printf("Process %d: exit with %d\n", pid, status >> 8);
    else if ((status & 0xff00) == 0)
        printf("Process %d: killed with signal %d %s core file\n", pid, status & 0x7f,
               (status & 0x80) ? "with" : "without");
}

int main()
{
    signal(SIGCHLD, handler);
    pid_t res = fork();

    if (res < 0)
    {
        printf("Failed with forking 1\n");
        exit(-1);
    }
    else if (res == 0)
    {
        exit(200);
    }

    if (res < 0)
    {
        printf("Failed with forking 2\n");
        exit(-1);
    }
    else if (res == 0)
    {
        while (1)
            ;
    }

    while (1)
        ;
    return 0;
}