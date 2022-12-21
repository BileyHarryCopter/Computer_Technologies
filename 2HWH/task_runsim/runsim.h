#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//  for signals
#include <wait.h>
#include <signal.h>
#include <stdbool.h>

enum KEYS
{
    STOP = 2,
    MAXLEN = 100,
    OPTIMAL_NUMBER_OF_CMD = 10
};

int getnumb(const char *str);
int getcmd(char **cmd);
int delcmd(char *cmd);
int execute(const char *cmd);
int run(const char *cmd);
void handler(int signal);
void handler_kill_all_process(int signal);