#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
//  for signals
#include <wait.h>
#include <signal.h>
//  for clone
#include <sched.h>

#define STACK_SIZE (1024 * 1024)
char child_stack[STACK_SIZE];

typedef struct arg_namespace
{
    int limit_numb;
    int active_cmd_numb;
    int *child_pids;
} arg_t;

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
int run(arg_t *arg_namespace);
void handler(int signal);
void handler_kill_all_process(int signal);