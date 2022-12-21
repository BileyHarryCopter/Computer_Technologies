#include "runsim.h"

int status_cmd = 0;

int main(char argc, char *argv[])
{
    int mode = 0;
    int limit_numb = 0;
    if (argc == 1)
    {
        printf("Basic mode: number of commands = 10, showing active commands in run time: 0\n");
        mode = NO_LIST;
        limit_numb = OPTIMAL_NUMBER_OF_CMD;
    }
    else if (argc == 2)
    {
        mode = getnumb(argv[1]);
        limit_numb = OPTIMAL_NUMBER_OF_CMD;
        printf("Average mode: number of commands = 10, showing active commands in run time: %d\n", mode);
    }
    else if (argc == 3)
    {
        mode = getnumb(argv[1]);
        limit_numb = getnumb(argv[2]);
        printf("Custom mode: number of commands = %d, showing active commands in run time: %d\n", limit_numb, mode);
    }

    if (mode == LIST)
    {
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
    }
    else
    {
        char *cmd = (char *)calloc(MAXLEN, sizeof(char));

        for (; true;)
        {
            fflush(stdin);
            memset(cmd, '\0', MAXLEN);

            int status = getcmd(&cmd);

            if ((status == STOP) || (strcmp(cmd, "$") == 0))
            {
                fflush(stdin);
                memset(cmd, '\0', MAXLEN);
                break;
            }
            else if (status_cmd == limit_numb)
            {
                printf("Sorry, the limit of active process has been approached. Wait a minute\n");
                continue;
            }
            else
            {
                execute(cmd);
            }
        }
        free(cmd);
    }

    return 0;
}

int run()
{
    int *arg = shminit("runsim.c");

    char *cmd = (char *)calloc(MAXLEN, sizeof(char));

    for (; true;)
    {
        fflush(stdin);
        memset(cmd, '\0', MAXLEN);

        status_cmd = getcmd(&cmd);

        if ((status_cmd == STOP) || (strcmp(cmd, "$") == 0))
        {
            fflush(stdin);
            memset(cmd, '\0', MAXLEN);
            break;
        }
        else if (strcmp(cmd, "--list") == 0)
        {
            printf("\nParent's pid: 1\n");

            for (int i = LIMIT + 1, capacity = 2 * OPTIMAL_NUMBER_OF_CMD;
                 i < capacity; i++)
            {
                if (arg[i] != 0)
                    printf("Child's pid: %d\n", arg[i]);
            }

            fflush(stdin);
            memset(cmd, '\0', MAXLEN);

            continue;
        }
        else if (arg[CURRENT] == arg[LIMIT])
        {
            printf("Sorry, the limit of active process has been approached. Wait a minute\n");
            continue;
        }
        else
            execute_res(cmd);
    }

    free(cmd);
    return 0;
}

int execute_res(const char *cmd)
{
    int pid = 0;
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
        pid = execute_list(cmd);
        for (int i = LIMIT + 1, capacity = 2 * OPTIMAL_NUMBER_OF_CMD;
             i < capacity; i++)
        {
            if (arg[i] == pid)
            {
                arg[i] = 0;
                break;
            }
        }
        arg[CURRENT]--;
        printf("Number of active program: %d\n", arg[CURRENT]);
    }

    return 0;
}

int execute_list(const char *cmd)
{
    int *arg = shminit("runsim.c");

    pid_t aux_res = fork();

    if (aux_res < 0)
    {
        printf("Can't create child's process\n");
        exit(-1);
    }
    else if (aux_res == 0)
    {
        for (int i = LIMIT + 1, capacity = 2 * OPTIMAL_NUMBER_OF_CMD;
             i < capacity; ++i)
        {
            if (arg[i] == 0)
            {
                arg[i] = getpid();
                break;
            }
        }

        int res_exec = execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
        if (res_exec < 0)
            printf("Error execute\n");
    }
    else
    {
        pid_t pid = getpid();
        for (int i = LIMIT + 1, capacity = 2 * OPTIMAL_NUMBER_OF_CMD;
             i < capacity; ++i)
        {
            if (arg[i] == 0)
            {
                arg[i] = pid;
                break;
            }
        }

        int status;
        pid_t candidate = wait(&status);

        for (int i = LIMIT + 1, capacity = 2 * OPTIMAL_NUMBER_OF_CMD;
             i < capacity; i++)
        {
            if (arg[i] == pid)
            {
                arg[i] = 0;
                break;
            }
        }

        return candidate;
    }
}

int execute(const char *cmd)
{
    printf("Command: \"%s\"\n", cmd);
    status_cmd++;
    printf("Number of active program: %d\n", status_cmd);

    signal(SIGCHLD, handler);

    pid_t aux_res = fork();

    if (aux_res < 0)
    {
        printf("Can't create child's process\n");
        exit(-1);
    }
    else if (aux_res == 0)
    {
        int res_exec = execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
        if (res_exec < 0)
            printf("Error execute\n");
    }
}

int *shminit(const char *file_name)
{
    int *arg = 0;
    int shmid = 0;
    int size = 2 * OPTIMAL_NUMBER_OF_CMD;
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

void handler(int signal)
{
    if (signal == SIGCHLD)
    {
        status_cmd--; //  reducing number of active processes
        printf("Number of active program: %d\n\n", status_cmd);
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
    {
        fflush(stdin);
        memset(*cmd, '\0', MAXLEN);
        return STOP;
    }
}
