#include "runsim.h"

int number_active_cmd = 0;

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

    char *cmd = (char *)calloc(MAXLEN, sizeof(char));

    for (; true;)
    {
        getcmd(&cmd);

        if (number_active_cmd == limit_numb)
        {
            printf("Sorry, the limit of active process has been approached. Wait a minute\n");
            continue;
        }
        else
        {
            printf("Command: \"%s\"\n", cmd);
            number_active_cmd++;
            printf("Number of active program: %d\n", number_active_cmd);
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
        number_active_cmd--; //  reducing number of active processes
        printf("Number of active program: %d\n\n", number_active_cmd);
    }
}

int execute(const char *cmd)
{
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