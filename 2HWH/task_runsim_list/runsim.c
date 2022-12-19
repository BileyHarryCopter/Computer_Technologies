#include "runsim.h"

int status_cmd = 0;

int main(char argc, char *argv[])
{
    int limit_numb = 0;
    if (argc == 1)
        limit_numb = OPTIMAL_NUMBER_OF_CMD;
    else
        limit_numb = getnumb(argv[1]);

    int *arg = shminit("runsim.c");
    arg[CURRENT] = 0;
    arg[LIMIT] = limit_numb;

    pid_t child_pid = clone(run, child_stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD, (void *)NULL);
    if (child_pid == -1)
    {
        printf("Can't create new namespace\n");
        exit(-1);
    }

    int wstatus;
    wait(&wstatus);

    if (shmctl(arg[SHMID], IPC_RMID, NULL) < 0)
    {
        printf("Can't delete shmem. This is trully hopeless\n");
        exit(-1);
    }

    return 0;
}

int run()
{
    signal(SIGCHLD, handler);

    int *arg = shminit("runsim.c");

    char *cmd = (char *)calloc(MAXLEN, sizeof(char));

    for (; true;)
    {
        fflush(stdin);
        memset(cmd, '\0', MAXLEN);

        status_cmd = getcmd(&cmd);

        if (strcmp(cmd, "$") == 0)
            break;
        else if (strcmp(cmd, "--list") == 0)
        {
            printf("\nParent's pid: %d\nChild's pid: %d\n", getppid(), getpid());

            for (int i = 0; i < arg[CURRENT]; i++)
                printf("Child's pid: %d\n", arg[LIMIT + 1 + i]);

            continue;
        }
        else if (arg[CURRENT] == arg[LIMIT])
        {
            printf("Sorry, the limit of active process has been approached. Wait a minute\n");
            continue;
        }
        else
            execute(cmd);
    }

    free(cmd);
    return 0;
}

int execute(const char *cmd)
{
    int *arg = shminit("runsim.c");

    printf("Command: \"%s\"\n", cmd);
    arg[CURRENT] += 1;
    printf("Number of active program: %d\n", arg[CURRENT]);

    pid_t aux_res = fork();

    if (aux_res < 0)
    {
        printf("Can't create child's process\n");
        exit(-1);
    }
    else if (aux_res == 0)
    {
        arg[LIMIT + arg[CURRENT]] = getpid();
        int res_exec = execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
        if (res_exec < 0)
            printf("Error execute\n");
    }

    return 0;
}

void handler(int signal)
{
    int *arg = shminit("runsim.c");
    if (signal == SIGCHLD)
    {
        arg[CURRENT] -= 1; //  reducing number of active processes
        printf("Number of active program: %d\n\n", arg[CURRENT]);
    }
}

int *shminit(const char *file_name)
{
    int *arg = 0;
    int shmid = 0;
    int size = 2 * OPTIMAL_NUMBER_OF_CMD * sizeof(int);
    key_t key;

    if ((key = ftok("runsim.c", 0)) < 0)
    {
        printf("Can't generate a key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        if (errno != EEXIST)
        {
            printf("Can't create shmem\n");
            exit(-1);
        }
        else if ((shmid = shmget(key, size, 0)) < 0)
        {
            printf("Can't find shmem\n");
            exit(-1);
        }
    }

    if ((arg = shmat(shmid, NULL, 0)) < 0)
    {
        printf("Can't map shmem\n");
        exit(-1);
    }

    arg[SHMID] = shmid;

    return arg;
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
}
