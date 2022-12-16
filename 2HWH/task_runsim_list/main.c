#include "runsim.h"

arg_t *arg_namespace = NULL;

int main(char argc, char *argv[])
{
    int limit_numb = 0;
    if (argc == 1)
    {
        printf("Number of active process is set by default: %d\n", OPTIMAL_NUMBER_OF_CMD);
        limit_numb = OPTIMAL_NUMBER_OF_CMD;
    }
    else
        limit_numb = getnumb(argv[1]);

    arg_namespace = (arg_t *)calloc(1, sizeof(arg_t));
    arg_namespace->active_cmd_numb = 0;
    arg_namespace->limit_numb = limit_numb;
    arg_namespace->child_pids = (int *)calloc(limit_numb, sizeof(int));

    //  create a clone and lauch run in them
    pid_t child_pid = clone(run, child_stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD, &arg_namespace);

    if (child_pid == -1)
    {
        printf("Can't create new namespace\n");
        return -1;
    }

    int wstatus;
    wait(&wstatus);

    free(arg_namespace->child_pids);
    free(arg_namespace);

    return 0;
}

int run(arg_t *arg_namespace)
{
    char *cmd = (char *)calloc(MAXLEN, sizeof(char));

    for (; true;)
    {
        getcmd(&cmd);

        if (strcmp(cmd, "--list") == 0)
        {
            printf("\nParent's pid: %d\nChild's pid: %d\n", getppid(), getpid());

            for (int i = 0; i < arg_namespace->active_cmd_numb; i++)
            {
                printf("Child pid #%d: %d\n", i, arg_namespace->child_pids[i]);
            }
            fflush(stdin);
            continue;
        }

        if (arg_namespace->active_cmd_numb == arg_namespace->limit_numb)
        {
            printf("Sorry, the limit of active process has been approached. Wait a minute\n");
            continue;
        }
        else
        {
            execute(cmd);
        }
        memset(cmd, '\0', MAXLEN);
        fflush(stdin);
    }
    free(cmd);

    return 0;
}

void handler(int signal)
{
    if (signal == SIGCHLD)
    {
        arg_namespace->active_cmd_numb--; //  reducing number of active processes
        printf("Number of active program: %d\n\n", arg_namespace->active_cmd_numb);
    }
}

int execute(const char *cmd)
{
    printf("Command: \"%s\"\n", cmd);
    arg_namespace->active_cmd_numb++;
    printf("Number of active program: %d\n", arg_namespace->active_cmd_numb);

    signal(SIGCHLD, handler);

    pid_t aux_res = fork();

    if (aux_res < 0)
    {
        printf("Can't create child's process\n");
        exit(-1);
    }
    else if (aux_res == 0)
    {
        arg_namespace->child_pids[arg_namespace->active_cmd_numb] = getpid();
        int res_exec = execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
        if (res_exec < 0)
            printf("Error execute\n");
    }
}

int getnumb(const char *str)
{
    int numb = 0;
    for (int i = 0, strsize = strlen(str); i < strsize; i++)
    {
        numb += str[i] - '0';
        numb *= 10;
    }
    return numb / 10;
}

int getcmd(char **cmd)
{
    char symb = getc(stdin);
    for (char i = 0; (symb != EOF) && (symb != '\n') && (i < MAXLEN); i++)
    {
        (*cmd)[i] = symb;
        symb = getc(stdin);
    }
    if (symb == EOF)
        kill(-getpid(), SIGINT);
}