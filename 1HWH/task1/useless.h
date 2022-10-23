#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

enum errors
{
    EMPTY_FILE = 0,
    OPEN_ER = -1,
    CLOSE_ER = -1,
    COM_NUMB = 10,
    MAX_LENGTH = 50,
    FLAGS_NUMB = 10
};

int file_opening(const char *file_name);
int file_closing(const int des);
size_t file_size(const int des);
char *get_command(char *flags[]);
int get_sec(char *buff, int *pos);
void passspace(char *buff, int *pos);
char *command_init(char *buff, int *pos);
char **flags_init(char *buff, int *pos, char *command);
void free_flags(char **flags);
void execute_func(const int sec, char *command, char *flags[]);
int execution(const char *file_name);