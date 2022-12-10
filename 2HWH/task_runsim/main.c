#include "runsim.h"

int number_active_cmd = 0;

int main(char argc, char *argv[])
{
    signal(SIGCHLD, handler);
    // signal(SIGINT, handler_kill_all_process);

    for (int limit_numb = getnumb(argv[1]); true;)
        execute(limit_numb);
    return 0;
}

void handler(int signal)
{
    if (signal == SIGCHLD)
        --number_active_cmd; //  reducing number of active processes
}

void handler_kill_all_process(int signal)
{
    pid_t pid = getpid();
    if (signal == SIGINT)
        kill(-pid, SIGKILL); //  sending signal of killing all of process in the group
}

int execute(int limit_numb)
{
    printf("NUMBER OF ACTIVE PROGRAM: %d\n", number_active_cmd);

    //  before all of this staff we should understand number of active commands
    if (number_active_cmd == limit_numb)
    {
        printf("Sorry, the limit of active process at the same time was approached. Wait a minute\n");
        waitpid(0, NULL, 0); //  waiting for a process which has finished his work
    }
    //  after checking

    char *cmd = 0;
    getcmd(&cmd);
    ++number_active_cmd;

    pid_t res = fork();

    if (res < 0)
    {
        printf("Can't create child's process\n");
        exit(-1);
    }
    else if (res > 0) //  inside the parent's process
    {
        int status;
        wait(&status);
        delcmd(cmd);
    }
    else //  inside the child's process
    {
        printf("NUMBER OF ACTIVE PROGRAM: %d\n", number_active_cmd);
        // execl(cmd, cmd, NULL); //  execute a program with infinite circle
        execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
    }
}