#define _GNU_SOURCE
//  standart
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//  system calls
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
//  for signals
#include <wait.h>
#include <signal.h>
//  for clone
#include <sched.h>
//  for shmem
#include <sys/ipc.h>
#include <sys/shm.h>
//  for errors
#include <errno.h>

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
    SHMID = 0,
    CURRENT = 1,
    LIMIT = 2,
    STOP = 2,
    MAXLEN = 100,
    OPTIMAL_NUMBER_OF_CMD = 10
};

int run();
int delcmd(char *cmd);
int getcmd(char **cmd);
void handler(int signal);
int getnumb(const char *str);
int execute(const char *cmd);
int *shminit(const char *file_name);
